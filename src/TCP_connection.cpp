#include "TCP_connection.h"
#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <fstream>
#include <unistd.h>
#include <Functions.h>
#include <experimental/filesystem>
#include <cstdio>

#define SERVER_PORT 51000

using namespace std;
namespace fs=std::experimental::filesystem;

bool TCP_listener(string &name, int &id, int &option)
{
    struct sockaddr_in serverTCP;
    serverTCP.sin_family = AF_INET;
    serverTCP.sin_port=htons(SERVER_PORT);
    serverTCP.sin_addr.s_addr=htons(INADDR_ANY);

    const int socket_ = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_<0)
    {
        cout<<"Socket error"<<endl;
        return false;
    }

    socklen_t len = sizeof(serverTCP);

    if(bind (socket_,(struct sockaddr *)&serverTCP,len)<0)
    {
        cout<<"Bind error"<<endl;
        return false;
    }

    if(listen(socket_,1)<0)
    {
        return false;
        cout<<"Listen error"<<endl;

    }


    struct sockaddr_in clientTCP;

    const int clnsck=accept(socket_,(struct sockaddr *) &clientTCP, &len);

    if(clnsck<=0)
    {
        cout<<"Client error"<<endl;
        return false;
    }

    char buffer[64]= {};

    if(recv(clnsck,buffer, sizeof(buffer),0)<=0)
    {
        cout<<"Recieve error"<<endl;
        return false;
    }

    int name_lenght = atoi(buffer);
    char buffer_name[name_lenght]= {};


    send(clnsck,"0",1,0);
    if(recv(clnsck,buffer_name, sizeof(buffer_name),0)<=0)
    {
        cout<<"Recieve error"<<endl;
        return false;
    }


    string tmp_msg = buffer_name;
    int code = atoi(tmp_msg.substr(0,1).c_str());
    int grp_ID = atoi (tmp_msg.substr(1,tmp_msg.find('.')-1).c_str());
    tmp_msg=tmp_msg.substr(tmp_msg.find('.')+1,name_lenght -tmp_msg.find('.')+1);

    id=grp_ID;
    option = code;

    if(code==0)
    {
        send(clnsck,"0",1,0);
        if(recv(clnsck,buffer, sizeof(buffer),0)<=0)
        {
            cout<<"Recieve error"<<endl;
            return false;
        }


        long file_len = atoi(buffer);
        char buffer_ti[file_len]= {};
        char tmp_buf[1024]= {};
        long counter=0;
        long tmp_cnt=file_len;

        while(counter<file_len)
        {
            send(clnsck,"0",1,0);
            tmp_cnt=file_len-counter;

            if(tmp_cnt>=1024)
            {

                recv(clnsck,tmp_buf, 1024,0);
                for(int i =0; i<1024; i++)
                {
                    buffer_ti[counter]=tmp_buf[i];
                    counter++;
                }

            }
            else
            {
                recv(clnsck,tmp_buf, tmp_cnt,0);
                for (int i=0; i<tmp_cnt; i++)
                {
                    buffer_ti[counter]=tmp_buf[i];
                    counter++;
                }
            }
        }

        string path ="/usr/TI/slides/"+tmp_msg+".zip";

        ofstream file(path);
        file.write(buffer_ti,file_len);
        name=tmp_msg;
    }

    if (code==1)
    {
        string path_s="/usr/TI/slides/"+tmp_msg+".zip";
        //string path_s="/usr/TI/slides/test.txt";

        int file_s=fs::file_size(path_s);
        char file_data[file_s];
        char tmp_char[1024];

        ifstream file(path_s, ios::binary);
        file.read(file_data,file_s);

        int counter =0;
        cout<<file_s<<endl;
        char buffor[64]= {};
        sprintf(buffor,"%d",file_s);

        send(clnsck,buffor,64,0);




        while (counter<file_s)
        {

         if(recv(clnsck,buffer, 1,0)<=0)
        {
            cout<<"Recieve error"<<endl;
            return false;
        }
            if(file_s - counter> 1024)
            {
                for (int i =0; i<1024; i++)
                {
                    tmp_char[i]=file_data[counter];
                    counter++;

                }
                send(clnsck,tmp_char,1024,0);
            }
            else
            {
                int tmp_val = file_s-counter;
                char m_tmp_char[tmp_val];
                for (int i = 0; i<tmp_val; i++)
                {
                    m_tmp_char[i]=file_data[counter];
                    counter++;
                }
                send(clnsck,m_tmp_char,tmp_val,0);
            }

        }


    }

    close(clnsck);
    close (socket_);


    return true;
}
