#include <iostream>
#include<fstream>
#include<vector>
#include<string.h>
using namespace std;

struct Alumno
{
    char codigo [5];
    char nombre [11];
    char apellidos [20];
    char carrera [15];
};

class FixedRecord{
    string filename;
public:
    explicit FixedRecord(const string &filename) : filename(filename) {}

    vector<Alumno> load() {
        cout<<"Loading file"<<endl;
        vector<Alumno> vecload;

        ifstream file;

        char apellido1[20];
        char apellido2[20];
        char del[2] = " ";

        string ap;
        file.open(filename, ios::in); //Se abre el archivo en modo lectura

        if (file.fail()) {
            cout << "Not opened" << endl;
            exit(1);
        }

        while (!file.eof()) {
            Alumno Student;
            file >> Student.codigo;
            file >> Student.nombre;
            file >> apellido1;
            file >> apellido2;


            strcat(apellido1, del);
            strcat(apellido1, apellido2);

            strcpy(Student.apellidos, apellido1);
            file >> Student.carrera;

            vecload.push_back(Student);
        }

        file.close();
        return vecload;
    }

    void add(Alumno record){
        cout<<"Adding file..."<<endl;
        ofstream file;

        file.open(filename, ios::out | ios::app);
        file <<"\n"<< record.codigo<<" "<<record.nombre<<"\t\t"<<record.apellidos<<"\t\t"<<record.carrera;
        file.close();
    }

    Alumno readRecord(int pos){
        ifstream file;
        Alumno rec;
        int index = 0;
        char apellido1[20];
        char apellido2[20];
        char del[2] = " ";
        file.open(filename, ios::in); //Se abre el archivo en modo lectura

        while (!file.eof() && index <= pos) {

            file >> rec.codigo;
            file >> rec.nombre;
            file >> apellido1;
            file >> apellido2;


            strcat(apellido1, del);
            strcat(apellido1, apellido2);

            strcpy(rec.apellidos, apellido1);
            file >> rec.carrera;
            index++;
        }

        return rec;


    }




};

int main() {
    string filename = "../datos1.txt";
    vector<Alumno> vec;

    FixedRecord rec(filename);
    vec = rec.load();


    for(auto i:vec){
        cout<<i.codigo<<" "<<i.nombre<<"\t"<<i.apellidos<<"\t"<<i.carrera<<endl;
    }

    Alumno alumn;
    strcpy(alumn.codigo, "0010");
    strcpy(alumn.nombre, "Oscar");
    strcpy(alumn.apellidos, "Chu Lao");
    strcpy(alumn.carrera, "DataScience");


    rec.add(alumn);
    vec = rec.load();

    for(auto i:vec){
        cout<<i.codigo<<" "<<i.nombre<<" "<<i.apellidos<<" "<<i.carrera<<endl;
    }


    Alumno a = rec.readRecord(2);
    cout<<"Nombre de la pos2: "<<a.nombre<<endl;
    return 0;
}
