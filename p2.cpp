#include <array>
#include <vector>
#include <iostream>
#include <cstring>
#include <fstream>
#include <list>

struct Alumno
{
    char codigo [5]{0};
    char nombre [11]{0};
    char apellidos [20]{0};
    char carrera [15]{0};
    int ciclo;
    float mensualidad;
};

std::ostream& operator <<(std::ostream& os, const Alumno& d)
{
    os << "Alumno:" << "\n";
    os << (const char*)d.codigo << "\n";
    os << (const char*)d.nombre << "\n";
    os << (const char*)d.apellidos << "\n";
    os << (const char*)d.carrera << "\n";
    os << d.ciclo << "\n";
    os << d.mensualidad << "\n" << std::endl ;

    return os;
}

std::array<char,sizeof(Alumno)> serializeAlumno(const Alumno& al)
{
    std::array<char,sizeof(Alumno)> data;
    memcpy(&data[0],&al,sizeof(Alumno));
    return data;
}

Alumno deserializeAlumno(std::array<char,sizeof(Alumno)> data)
{
    Alumno al;
    memcpy(&al,&data[0],sizeof(Alumno));
    return al;
}

class FixedRecord
{
private:
    std::string table;

    long int getPosFromIndex(int index)
    {   //     Head of free list + offset to propper place
        return sizeof(long int) + index*sizeof(Alumno);
    }

    long int getIndexFromPosition(int position)
    {
        return (position-sizeof(long int))/sizeof(Alumno);
    }
public:
    FixedRecord(std::string filename):table(filename)
    {
        // check file exists and store the thingy
        std::fstream file;
        file.open(table,std::ios::in | std::ios::binary);
        if (file.fail())
        {
            file.clear();
            //handle it
            std::cout << "[INFO] FixedRecord: Creating file... " << table << std::endl;
            file.open(table,std::ios::out | std::ios::trunc | std::ios::binary);

            long int len = sizeof(long int);
            file.write((char*)&len,sizeof(long int));
        }
        file.close();
    }

    std::vector<Alumno> load()
    {
        std::fstream file;
        file.open(table,std::ios::in| std::ios::binary);

        // first build the deleteds set
        std::list<long int> deleteds;
        file.seekg(0);
        while(!file.eof())
        {
            long int empty_space;
            file.read((char*)&empty_space,sizeof(long int));
            if (!file.eof())
            {
                deleteds.push_back(getIndexFromPosition(empty_space));
                file.seekg(empty_space);
            }
            else
                break;
        }
        deleteds.sort();

        file.clear();
        std::vector<Alumno> rows;
        long int i = 0;
        while(!file.eof())
        {
            if (i==*deleteds.begin())
            {
                //skip over
                deleteds.pop_front();
                i++;
                continue;
            }
            file.seekg(getPosFromIndex(i));

            std::array<char,sizeof(Alumno)> data;
            file.read((char*)&data[0],sizeof(std::array<char,sizeof(Alumno)>));
            if (file.eof()) break;
            Alumno al = deserializeAlumno(data);

            rows.push_back(al);
            i++;
        }
        file.close();
        return rows;
    }

    void add(Alumno& al)
    {
        std::fstream file;
        file.open(table, std::ios::in | std::ios::out |std::ios::binary);
        file.seekg(0,std::ios::beg); // go to start of file to find start of freelist
        long int writepos;
        file.read((char*)&writepos,sizeof(long int)); // get value of start of freelist
        if (file.good() != 1)
        {
            std::cout << "[ERROR] FixedRecord Corrupted file" << std::endl;
            throw "corrupt file";
        }
        long int newhead; //get the new head
        file.seekg(writepos);
        file.read((char*)&newhead,sizeof(long int));
        if (file.eof()) // head pointed to end of file!
        {
            newhead = -1;
            file.clear(); //get rid of the eof
        }
        //serialize alumno
        std::array<char,sizeof(Alumno)> serialized = serializeAlumno(al);
        file.write(&serialized[0],sizeof(serialized));
        if (newhead == -1)
            newhead = file.tellp();
        file.seekp(0);
        file.write((char*)&newhead,sizeof(long int));
        file.close();
    }

    Alumno readRecord(int index)
    {
        std::fstream file;
        file.open(table,std::ios::in | std::ios::out |std::ios::binary);
        file.seekg(getPosFromIndex(index));

        std::array<char,sizeof(Alumno)> data;
        file.read((char*)&data[0],sizeof(std::array<char,sizeof(Alumno)>));
        Alumno al = deserializeAlumno(data);
        file.close();
        return al;
    }

    bool delete_(int index)
    {
        //just pushfront into the deleteds list
        // make head point to this and point to ondhead in this
        std::fstream file;
        file.open(table,std::ios::in | std::ios::out |std::ios::binary);

        long int oldhead,newhead;
        file.seekg(0);
        file.read((char*)&oldhead,sizeof(long int));

        file.seekp(0);
        newhead = getPosFromIndex(index);
        file.write((char*)&newhead,sizeof(long int));
        file.seekp(newhead);
        file.write((char*)&oldhead,sizeof(long int));
        file.close();
        return file.good();

    }

};

void buildalumno(Alumno& a,const char* cod,const char* nom,const char* ap,const char* carr,int cic,int men)
{
    strcpy(a.codigo,cod);
    strcpy(a.nombre,nom);
    strcpy(a.apellidos,ap);
    strcpy(a.carrera,carr);
    a.ciclo = cic;
    a.mensualidad = men;
}
int main(void)
{
    Alumno a,b,c,d;
    buildalumno(a,"1999","Juan","Perez Sanches","Civil",2,1000);
    buildalumno(b,"2000","Pablo","Cabrera Salazar","Civil",1,1000);
    buildalumno(c,"2001","Osama","Bin Laden","Civil",5,911);
    buildalumno(d,"2002","Mao","Tze Tzung","Industrial",10,1500);

    FixedRecord recorder("text.dbt");
    recorder.add(a);
    recorder.add(b);
    recorder.add(c);
    recorder.add(d);
    std::cout << "------------------ 1 ----------------" << std::endl;
    for (auto i:recorder.load())
        std::cout << i;
    std::cout << "------------------ 2 ----------------" << std::endl;
    recorder.delete_(2);
    for (auto i:recorder.load())
        std::cout << i;
    std::cout << "------------------ 3 ----------------" << std::endl;
    std::cout << recorder.readRecord(3);
}