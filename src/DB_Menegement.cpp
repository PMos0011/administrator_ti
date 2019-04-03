#include "DB_Menegement.h"
#include <iostream>
#include <mysql/mysql.h>

using namespace std;

bool DB_Write(string name, int id)
{
    MYSQL mysql;
    mysql_init(&mysql);
    MYSQL_RES *id_query;
    MYSQL_ROW data_row;

    if(mysql_real_connect(&mysql, "localhost", "TI", "rciwiswd04", "TABLICA", 0, NULL, 0))
        cout<<"Connected"<<endl;
    else
    {
        cout<<"Connection Error"<<endl;
        return false;
    }

    mysql_select_db(&mysql, "TABLICA");

    string sendBuff="SELECT RS FROM WORK WHERE GRP_ID="+to_string(id);

    if(mysql_query(&mysql, sendBuff.c_str()))
    {
        cout<<"Write state Error 0"<<endl;
        mysql_close(&mysql);
        return false;
    }

    id_query=mysql_store_result(&mysql);
    data_row=mysql_fetch_row(id_query);
    int reset=atoi(data_row[0]);
    reset++;
    if(reset>9)
        reset=0;

    sendBuff="UPDATE WORK SET SLIDE = '"+name+"', RS = "+to_string(reset) + " WHERE GRP_ID= "+to_string(id);

    if(mysql_query(&mysql, sendBuff.c_str()))
    {
        cout<<"Write state Error 1 "<<endl;
        mysql_close(&mysql);
        return false;
    }

    mysql_free_result(id_query);
    mysql_close(&mysql);

    return true;
}

bool Saves_add(std::string name, int id)
{
MYSQL mysql;
    mysql_init(&mysql);

    if(mysql_real_connect(&mysql, "localhost", "TI", "rciwiswd04", "TABLICA", 0, NULL, 0))
        cout<<"Connected"<<endl;
    else
    {
        cout<<"Connection Error"<<endl;
        return false;
    }

    mysql_select_db(&mysql, "TABLICA");

    //string sendBuff="SELECT RS FROM WORK WHERE GRP_ID="+to_string(id);

    string sendBuff = "INSERT INTO SAVE (GRP_ID, NAME, DATE) VALUES ('"+to_string(id)+"', '"+name+"', CURRENT_TIMESTAMP)";

    if(mysql_query(&mysql, sendBuff.c_str()))
    {
        cout<<"Write state Error 0"<<endl;
        mysql_close(&mysql);
        return false;
    }

     mysql_close(&mysql);

    return true;

}

