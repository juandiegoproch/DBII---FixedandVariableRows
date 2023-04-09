#include <fstream>
#include <vector>
#include <iostream>
#include <string>

struct Matricula
{
    std::string codigo;
    std::string observaciones;
    int ciclo;
    float mensualidad;

    std::vector<char> serialize() const
    {
        // way it looks: <lenregister><len_codigo><codigo><len_observaciones><observaciones><ciclo><mensualidad>
        // size it takes: sizeof(long) + sizeof(long) + codigo.len() + sizeof(long) + observaciones.len() + sizeof(int) + sizeof(float) =
        // 3*sizeof(long) + codigo.len + observaciones(len) + sizeof(int) + sizeof(float)
        int registry_size = 3*sizeof(long int)+codigo.length()+observaciones.length()+sizeof(int)+sizeof(float); 
        // m is for metadata. ^ we skip the first long because its easier to store "how many bytes till end of reg"
        long m_codigo_offset,codigo_offset;
        m_codigo_offset = sizeof(long); // <sizeof_reg>
        codigo_offset = sizeof(long)*2; // <sizeof_reg><sizeof_codigo>

        long m_observaciones_offset,observaciones_offset;
        m_observaciones_offset = codigo_offset + codigo.size(); // ...codigo>
        observaciones_offset = m_observaciones_offset + sizeof(long); // ...codigo><lenof observ> 

        long ciclo_offset = observaciones_offset + observaciones.size();
        long mensualidad_offset = ciclo_offset + sizeof(int);

        long cod_size, observ_size;
        cod_size = codigo.size();
        observ_size = observaciones.size();
        

        std::vector<char> serialized(registry_size);
        memcpy(&serialized[0],&registry_size, sizeof(long int));

        memcpy(&serialized[m_codigo_offset],&cod_size,sizeof(long));
        memcpy(&serialized[codigo_offset],codigo.c_str(),cod_size);

        memcpy(&serialized[m_observaciones_offset],&observ_size,sizeof(long));
        memcpy(&serialized[observaciones_offset],observaciones.c_str(),observ_size);

        memcpy(&serialized[ciclo_offset],&ciclo,sizeof(int));

        memcpy(&serialized[mensualidad_offset],&mensualidad,sizeof(float));

        return serialized;
    }

    Matricula& deserialize(const std::vector<char>& serialized,int offset = sizeof(long))
    {
        const char* start = &serialized[0] + offset; //ignore the size of the register
        
        // leer codigo
        int codlen;
        memcpy(&codlen,start,sizeof(long));
        start+=sizeof(long);

        char* temp_cstring = new char[codlen + 1]{0};
        memcpy(temp_cstring,start,codlen);
        codigo = (const char*)temp_cstring;
        start+=codlen;
        delete temp_cstring;

        // leer obsrvaciones
        int obslen;
        memcpy(&obslen,start,sizeof(long));
        start+=sizeof(long);

        temp_cstring = new char[obslen + 1]{0};
        memcpy(temp_cstring,start,obslen);
        observaciones = (const char*)temp_cstring;
        start+=obslen;
        delete temp_cstring;

        // leer ciclo
        memcpy(&ciclo,start,sizeof(int));
        start+= sizeof(int);
        // leer observaciones
        memcpy(&mensualidad,start,sizeof(int));
        start+= sizeof(float);

        return *this;
    }
};

void add(const Matricula& mat,std::string filename = "p4db.bin",std::string meta_filename = "p4db.met")
{
    auto serialized = mat.serialize();
    
    std::fstream file,meta;
    file.open(filename,std::ios::out | std::ios::app | std::ios::ate | std::ios::binary);

    int startofreg = file.tellp();

    file.write((char*)&serialized[0],serialized.size());
    file.close();



    meta.open(meta_filename,std::ios::out | std::ios::app | std::ios::ate | std::ios::binary);
    meta.write((char*)&startofreg,sizeof(int));
    meta.close();

}

Matricula readRecord(int index,std::string filename = "p4db.bin",std::string meta_filename = "p4db.met")
{
    std::fstream file,meta;

    meta.open(meta_filename);

    meta.seekg(index*sizeof(int));

    int pos;
    meta.read((char*)&pos,sizeof(int));

    meta.close();


    int sizeofrecord;
    file.open(filename);
    file.seekg(pos);
    file.read((char*)&sizeofrecord,sizeof(int));
    file.seekg(pos);

    std::vector<char> data(sizeofrecord);

    file.read((char*)&data[0],sizeofrecord);

    file.close();

    Matricula tmp;
    tmp.deserialize(data);

    return tmp;

}

std::vector<Matricula> load(std::string filename = "p4db.bin",std::string meta_filename = "p4db.met")
{
    std::vector<Matricula> tmp_vec;
    std::fstream meta;
    meta.open(meta_filename,std::ios::out | std::ios::app | std::ios::ate | std::ios::binary);
    int qty = meta.tellg()/sizeof(int);
    meta.close();

    for (int i = 0; i<qty; i++)
    {
        tmp_vec.push_back(readRecord(i,filename,meta_filename));
    }

    return tmp_vec;
}

std::ostream& operator<<(std::ostream& os, const Matricula& m)
{
    os << "Matricula: " << std::endl;
    os << m.codigo << std::endl;
    os << m.observaciones << std::endl;
    os << m.ciclo << std::endl;
    os << m.mensualidad << std::endl << std::endl;
    return os;
}


int main(void)
{
    Matricula a {"2021","Ninguna",3,2000.3};
    Matricula b;

    std::cout << "------------ 1 -------------" << std::endl;
    add(a);
    b = readRecord(0);

    std::cout << a;
    std::cout << b;

    std::cout << "------------ 2 -------------" << std::endl;

    Matricula c {"2000","esta gordo",5,2500.3};
    add(c);

    for (auto i:load())
    {
        std::cout << i;
    }

    return 0;
}
