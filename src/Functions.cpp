#include "Functions.h"
#include <iostream>
#include <zip.h>
#include <string>
#include <cstring>
#include <fstream>
#include <cstdint>
#include <experimental/filesystem>


using namespace std;
namespace fs=std::experimental::filesystem;

bool Unzip_file(string name, string &short_name)
{
    fs::path dir("/usr/TI");
    fs::create_directory(dir/"temp");

    string path_s="/usr/TI/slides/"+name+".zip";
    int path_lenght = path_s.size();
    name=name.substr(0,name.find('.'));
    char path[path_lenght+1];
    strcpy(path,path_s.c_str());
    int err=0;
    zip *z = zip_open(path,0,&err);
    int file_count = zip_get_num_entries(z,ZIP_FL_UNCHANGED);

    if (file_count>2)
    {

        struct zip_stat zipStat;
        zip_stat_init(&zipStat);
        char *content;
        zip_file *f;
        string save_path;
        string tmp_name;


        for (int i=0; i<file_count; i++)
        {
            tmp_name=zip_get_name(z,i,ZIP_FL_UNCHANGED);
            save_path = "/usr/TI/temp/" + tmp_name;
            zip_stat_index(z,i,ZIP_FL_UNCHANGED,&zipStat);
            content = new char[zipStat.size];
            f=zip_fopen_index(z,i,ZIP_FL_UNCHANGED);
            zip_fread(f,content,zipStat.size);

            ofstream file(save_path);
            file.write(content,zipStat.size);
            delete[] content;
        }

        zip_close(z);

        short_name = name;
    }
    else
        return false;

    return true;
}


bool Crate_structure(string short_name, string name)
{

    fs::path dir("/usr/TI/temp");
    fs::path dir_slide("/var/www/html/TI/SLIDES");
    fs::path dir_json("/var/www/html/TI/DATA");
    name += ".json";
    fs::remove(dir/"json.txt");


    for (const auto & file_name : fs::directory_iterator(dir_json))
    {
        string file_json_name = file_name.path();
        string tmp_name=file_json_name.substr(file_json_name.find_last_of('/')+1);
        tmp_name=tmp_name.substr(0,tmp_name.find('.'));

        if (!tmp_name.compare(short_name))
        {
            fs::remove(file_json_name);
            break;
        }
    }

    fs::copy(dir/name,dir_json);
    fs::remove(dir/name);

    if(fs::is_directory(dir_slide/ short_name))
        fs::remove_all(dir_slide/ short_name);

    fs::create_directory(dir_slide/short_name);
    fs::copy(dir, dir_slide/short_name);
    fs::remove_all(dir);


    return true;
}


