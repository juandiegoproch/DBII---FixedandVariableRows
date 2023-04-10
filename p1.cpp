//
// Created by RyanO on 9/04/2023.
//
#include<iostream>
#include<fstream>
#include<vector>
#include<cstring>
using namespace std;

struct Alumno
{
    char codigo [5];
    char nombre [11];
    char apellidos [20];
    char carrera [15];
    //[][][][][][][]...
};


class FixedRecord {
    string filename;
public:
    FixedRecord(const string &filename) : filename(filename) {}

    vector<Alumno> load() {
        cout << "Loading file..." << endl;

        vector<Alumno> students;


        ifstream file;
        file.open(filename);

        if (file.fail()) {
            cout << "File failed." << endl;
        } else {
            while(!file.eof()) {
                Alumno Std;
                file.read((char *) Std.codigo, sizeof(Std.codigo));
                Std.codigo[sizeof(Std.codigo)-1] = 0;
                file.read((char *) Std.nombre, sizeof(Std.nombre));
                Std.nombre[sizeof(Std.nombre)-1] = 0;
                file.read((char *) Std.apellidos, sizeof(Std.apellidos));
                Std.apellidos[sizeof(Std.apellidos)-1] = 0;
                file.read((char *) Std.carrera, sizeof(Std.carrera));
                Std.carrera[sizeof(Std.carrera)-1] = 0;

                file.get();
                students.push_back(Std);

            }
        }
        file.close();
        return students;
    }

    void add(Alumno record){
        cout<<"Adding file..."<<endl;
        ofstream file;
        file.open(filename, ios::app);

        if (file.fail()) {
            cout << "File failed." << endl;
        } else {
            char codigo1 [5]{0};
            char nombre1 [11]{0};
            char apellidos1 [20]{0};
            char carrera1 [15]{0};
            strcpy(codigo1, record.codigo);
            strcpy(nombre1, record.nombre);
            strcpy(apellidos1, record.apellidos);
            strcpy(carrera1, record.carrera);

            for(int i=0;i<5;i++){
                if(codigo1[i]==0){
                    codigo1[i]=' ';
                }
            }
            for(int i=0;i<11;i++){
                if(nombre1[i]==0){
                    nombre1[i]=' ';
                }
            }
            for(int i=0;i<20;i++){
                if(apellidos1[i]==0){
                    apellidos1[i]=' ';
                }
            }
            for(int i=0;i<15;i++){
                if(carrera1[i]==0){
                    carrera1[i]=' ';
                }
            }



            file.put('\n');
            file.write((char *) codigo1, sizeof(codigo1));
            file.write((char *) nombre1, sizeof(nombre1));
            file.write((char *) apellidos1, sizeof(apellidos1));
            file.write((char *) carrera1, sizeof(carrera1));


        }

        file.close();
    }

    Alumno readRecord(int pos){
        cout<<"Reading record..."<<endl;
        ifstream file;
        Alumno rec;
        int index = 0;
        file.open(filename, ios::in); //Se abre el archivo en modo lectura

        while (!file.eof() && index <= pos) {
            if(index != 0){
                file.get();
            }
            file.read((char *) rec.codigo, sizeof(rec.codigo));
            rec.codigo[sizeof(rec.codigo)-1] = 0;
            file.read((char *) rec.nombre, sizeof(rec.nombre));
            rec.nombre[sizeof(rec.nombre)-1] = 0;
            file.read((char *) rec.apellidos, sizeof(rec.apellidos));
            rec.apellidos[sizeof(rec.apellidos)-1] = 0;
            file.read((char *) rec.carrera, sizeof(rec.carrera));
            rec.carrera[sizeof(rec.carrera)-1] = 0;
            index++;
        }

        return rec;


    }

};


int main(){
    vector<Alumno> vec;
    FixedRecord rec("../pruebatexto.txt");
    vec = rec.load();

    for(auto i : vec){
        cout<<i.codigo<<" "<<i.nombre<<" "<<i.apellidos<<" "<<i.carrera<<endl;
    }


    Alumno alumn{"0008","Oscar","Prochazcka Zegarra","Bioingenieria"};
    rec.add(alumn);
    vec = rec.load();

    for(auto i : vec){
        cout<<i.codigo<<" "<<i.nombre<<" "<<i.apellidos<<" "<<i.carrera<<endl;
    }


    Alumno a = rec.readRecord(3);
    cout<<"Codigo de la pos: "<<a.codigo<<endl;
    cout<<"Nombre de la pos: "<<a.nombre<<endl;
    cout<<"Apellidos de la pos: "<<a.apellidos<<endl;
    cout<<"Carrera de la pos: "<<a.carrera<<endl;

    return 0;
}