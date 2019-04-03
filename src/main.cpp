#include <iostream>
#include "TCP_connection.h"
#include "Functions.h"
#include "DB_Menegement.h"


using namespace std;

int main()
{

    while (true)
    {

        string file_name;
        string short_name;
        int grp_id = -1;
        int option = -1;

        if(TCP_listener(file_name,grp_id,option));
        {
            if(option==0)
            {
                if(Unzip_file(file_name,short_name));
                {
                    Crate_structure(short_name,file_name);

                    int write_error = 0;

                    while(!DB_Write(file_name,grp_id))
                    {
                        write_error++;

                        if(write_error>3)
                            break;
                    }

                    while(!Saves_add(file_name,grp_id))
                    {
                        write_error++;

                        if(write_error>3)
                            break;
                    }
                }
            }

            else if(option==1)
            {
                cout<<"1"<<endl;
            }

            else
            {
                cout<<"error: "<< option<<endl;
            }
        }
    }
    return 0;
}
