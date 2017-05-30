#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "bignumber/bignumber.h"

using namespace std;

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int socketFileDescr, portNumber, n;
    struct sockaddr_in serverAddress;
    struct hostent *server;

    string result;

    char buffer[256];
    if (argc < 4)
    {
        fprintf(stderr, "usage %s hostname port file\n", argv[0]);
        exit(0);
    }
    else{
        printf("Arguments OK...\n");
    }
    cout<< "Testing..."<<endl;
    portNumber = atoi(argv[2]);
    socketFileDescr = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFileDescr < 0)
    {
        error("ERROR opening socket");
    }
    else{
        printf("Socket opened...\n");
    }

    server = gethostbyname(argv[1]);
    if (server == NULL)
    {
        fprintf(stderr, "ERROR, no such host\n");
        exit(0);
    }
    else{
        printf("Host founded...\n");
    }
    bzero((char *)&serverAddress, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serverAddress.sin_addr.s_addr, server->h_length);
    serverAddress.sin_port = htons(portNumber);
    if (connect(socketFileDescr, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
    {
        error("ERROR connecting");
    }
    else{
        printf("Connection OK...\n");
    }
    //read file specified in console
    ifstream fileToRead;
    cout<<"File to Open: ";
    cout<< argv[3]<<endl;
    fileToRead.open(argv[3]);
    char output[256];
    if (fileToRead.is_open())
    {
        printf("File Opened...\n");
        //i really need tofinda better way to serialize data to send via sockets
        while (!fileToRead.eof())
        {
            fileToRead >> output;
            string line;
            line = string(output);
            result += line + "/";
        }
        printf("File readed...\n");
        cout<< result<<endl;
    }
    else{
        error("ERROR opening file");
    }
    fileToRead.close();
    printf("File Closed...\n");
    
    n = write(socketFileDescr, result.data(), result.capacity());
    
    if (n < 0){
        error("ERROR writing to socket");
    }
    
    //wait for response    
    bzero(buffer, 256);
    n = read(socketFileDescr, buffer, 255);
    if (n < 0){
        error("ERROR reading from socket");
    }
    printf("%s\n", buffer);
    close(socketFileDescr);
    return 0;
}