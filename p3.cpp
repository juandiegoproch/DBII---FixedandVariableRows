//
// Created by RyanO on 5/04/2023.
//
#include <iostream>
#include<fstream>
#include<vector>
#include<string.h>
#include<sstream>
using namespace std;


struct Alumno
{
    string Nombre;
    string Apellidos;
    string Carrera;
    float mensualidad;
};

class VariableRecord{
    string filename;
public:
    explicit VariableRecord(const string &filename) : filename(filename) {}

    vector<Alumno> load() {
        cout<<"Loading file..."<<endl;
        vector<Alumno> students;
        Alumno alumn;
        string datatotal = "";
        string mensu = "";
        ifstream  loadfile;
        loadfile.open(filename, ios::in);

        if (loadfile.fail()) {
            cout << "Not opened" << endl;
            exit(1);
        }

        while(getline(loadfile, datatotal)){
            stringstream datosprocesados(datatotal);

            getline(datosprocesados, alumn.Nombre,'|');
            //cout<<"Nombre: "<<alumn.Nombre<<"\n";
            getline(datosprocesados, alumn.Apellidos,'|');
            //cout<<"Apellido: "<<alumn.Apellidos<<"\n";
            getline(datosprocesados, alumn.Carrera,'|');
            //cout<<"Carrera: "<<alumn.Carrera<<"\n";
            getline(datosprocesados, mensu);
            alumn.mensualidad = stof(mensu);
            //cout<<"Mensualidad: "<<alumn.mensualidad<<"\n";

            students.push_back(alumn);
        }

        return students;
    }

    void add(Alumno record){
        cout<<"Adding file..."<<endl;
        ofstream file;

        file.open(filename, ios::out | ios::app);
        file <<"\n"<< record.Nombre<<"|"<<record.Apellidos<<"|"<<record.Carrera<<"|"<<record.mensualidad;
        file.close();
    }

    Alumno readRecord(int pos){
        cout<<"Reading record..."<<endl;
        ifstream file;

        Alumno rec;

        string mensu;
        int index = 0;
        string datatotal;

        file.open(filename, ios::in); //Se abre el archivo en modo lectura

        while (getline(file, datatotal) && index <= pos) {
            if(index == pos) {
                stringstream datosprocesados(datatotal);
                getline(datosprocesados, rec.Nombre, '|');
                getline(datosprocesados, rec.Apellidos, '|');
                getline(datosprocesados, rec.Carrera, '|');
                getline(datosprocesados, mensu);
                rec.mensualidad = stof(mensu);
            }

            index++;
        }


        return rec;



    }

};



int main(){
    vector<Alumno> Estudiantes;
    VariableRecord vr("../datos2.txt");
    Estudiantes = vr.load();
    for(auto i: Estudiantes){
        cout<<"Nombre: "<<i.Nombre<<" | "<<"Apellido: "<<i.Apellidos<<" | "<<"Carrera: "<<i.Carrera<<" | "<<"Mensualidad: "<<i.mensualidad<<"\n";
    }

    
    Alumno alumn;
    alumn.Nombre = "Oscar";
    alumn.Apellidos = "Chu Lao Orrego";
    alumn.Carrera = "Data Science";
    alumn.mensualidad = 5555.55;

    vr.add(alumn);
    Estudiantes = vr.load();

    for(auto i: Estudiantes){
        cout<<"Nombre: "<<i.Nombre<<" | "<<"Apellido: "<<i.Apellidos<<" | "<<"Carrera: "<<i.Carrera<<" | "<<"Mensualidad: "<<i.mensualidad<<"\n";
    }



    Alumno a = vr.readRecord(4);

    cout<<"Nombre: "<<a.Nombre<<" | "<<"Apellido: "<<a.Apellidos<<" | "<<"Carrera: "<<a.Carrera<<" | "<<"Mensualidad: "<<a.mensualidad<<"\n";



    return 0;
}