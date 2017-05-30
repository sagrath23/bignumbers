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

/*
Function to handle errors
*/
void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int listenFd, portNo;
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

    //create client socket
    listenFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (listenFd < 0)
    {
        cerr << "Cannot open socket" << endl;
        return 0;
    }

    //try to find server
    server = gethostbyname(argv[1]);
    if (server == NULL)
    {
        cerr << "Host does not exist" << endl;
        return 0;
    }
    //clear svrAdd
    bzero((char *)&svrAdd, sizeof(svrAdd));
    //define the protocol
    svrAdd.sin_family = AF_INET;
    //copy the h_length bytes from svrAdd.sin_addr.s_addr to server->h_addr
    bcopy((char *)server->h_addr, (char *)&svrAdd.sin_addr.s_addr, server->h_length);
    //set the port in TCP Byte order
    svrAdd.sin_port = htons(portNo); 
    //try to connect to server
    int checker = connect(listenFd, (struct sockaddr *)&svrAdd, sizeof(svrAdd));

    if (checker < 0)
    {
        cerr << "Cannot connect!" << endl;
        return 0;
    }

    //send stuff to server
    char output[300];
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
            write(listenFd, output, strlen(output));    
        }
    }
    else
    {
        error("ERROR opening file");
    }
    //close file
    fileToRead.close();
    write(listenFd, "process", strlen("process"));
    //and wait for response
    for(;;){
        char s[300];
        //clear buffer
        bzero(s, 301);
        read(listenFd, s, 300);
        string tester (s);
        if(tester == "exit"){
            //break this loop
            cout<<"ending execution..."<<endl;
            break;
        }else{
            if(strlen(s) != 0){
                cout<<s<<endl;
            }
        }
    }
    //close socket and end process
    close(listenFd);
    return 0;
}