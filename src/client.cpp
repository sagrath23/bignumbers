#include <string.h>
#include <cstring>
#include <unistd.h>
#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <strings.h>
#include <stdlib.h>
#include <string>
#include <time.h>
#include <vector>

using namespace std;

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int listenFd, portNo;
    bool loop = false;
    struct sockaddr_in svrAdd;
    struct hostent *server;

    if (argc < 4)
    {
        cerr << "Syntax : ./client <host name> <port> <route_to_file>" << endl;
        return 0;
    }

    portNo = atoi(argv[2]);
    //validate port number
    if ((portNo > 65535) || (portNo < 2000))
    {
        cerr << "Please enter port number between 2000 - 65535" << endl;
        return 0;
    }

    //create client skt
    listenFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (listenFd < 0)
    {
        cerr << "Cannot open socket" << endl;
        return 0;
    }

    server = gethostbyname(argv[1]);

    if (server == NULL)
    {
        cerr << "Host does not exist" << endl;
        return 0;
    }

    bzero((char *)&svrAdd, sizeof(svrAdd));
    svrAdd.sin_family = AF_INET;

    bcopy((char *)server->h_addr, (char *)&svrAdd.sin_addr.s_addr, server->h_length);

    svrAdd.sin_port = htons(portNo);

    int checker = connect(listenFd, (struct sockaddr *)&svrAdd, sizeof(svrAdd));

    if (checker < 0)
    {
        cerr << "Cannot connect!" << endl;
        return 0;
    }

    //send stuff to server
    char output[300];
    int n;
    ifstream fileToRead;
    //open file and start to send each line
    cout << "File to Open: " << argv[3] << endl;
    fileToRead.open(argv[3]);
    if (fileToRead.is_open())
    {
        while (!fileToRead.eof())
        {
            //clean buffer
            bzero(output, 301);
            //read line
            fileToRead >> output;
            cout << output << endl;
            //and sent it to the server
            n = write(listenFd, output, strlen(output));    
        }
    }
    else
    {
        error("ERROR opening file");
    }
    //close file
    fileToRead.close();
    n = write(listenFd, "process", strlen("process"));
    //and wait for response
    for(;;){
        char s[300];
        //cin.clear();
        //cin.ignore(256, '\n');
        bzero(s, 301);
        read(listenFd, s, 300);
        string tester (s);
        if(tester == "exit"){
            break;
        }else{
            if(strlen(s) != 0){
                cout<<s<<endl;
            }
        }
        //cout << s << endl;
    }
    close(listenFd);
    return 0;
/*
    for (;;)
    {
        char s[300];
        //cin.clear();
        //cin.ignore(256, '\n');
        cout << "Enter stuff: ";
        bzero(s, 301);
        cin.getline(s, 300);
        
        write(listenFd, s, strlen(s));
    }*/
}