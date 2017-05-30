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

/*
Function to display controlled errors
*/
void error(const char *msg)
{
    perror(msg);
    exit(0);
}

/*
Main function of the client
*/
int main(int argc, char *argv[])
{
    //to start a socket in client, are defined: 
    int socketFileDescr;//the socket fileDescriptor
    int portNumber;//the port that will be used to send data to server
    int n;//and the number of bits sent
    //also, are defined
    struct sockaddr_in serverAddress;//an structure to handle the server's address
    struct hostent *server;//and a structure to handle the information of the server host
    
    string result;//this string will be store the information readed fron the file

    char buffer[256];//this is the buffer used to read server responses

    //to start the client, are validated the number of arguments introduced in the prompt
    if (argc < 4) // ./client host port route_to_file
    {
        fprintf(stderr, "usage %s hostname port file\n", argv[0]);
        exit(0);
    }
    else{
        printf("Arguments OK...\n");
    }

    portNumber = atoi(argv[2]);//convert to int the port number
    socketFileDescr = socket(AF_INET, SOCK_STREAM, 0);//and create a socket that use ip v.4 to stream data 

    if (socketFileDescr < 0)//validate the creation of the socket
    {
        error("ERROR opening socket");
    }
    else{
        printf("Socket opened...\n");
    }

    server = gethostbyname(argv[1]); //get the information of the server
    if (server == NULL)//and verify that the specified host exists
    {
        fprintf(stderr, "ERROR, no such host\n");
        exit(0);
    }
    else{
        printf("Host founded...\n");
    }
    //now, erase the data reated to server address
    bzero((char *)&serverAddress, sizeof(serverAddress));
    //and set it again using server reference
    serverAddress.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serverAddress.sin_addr.s_addr, server->h_length);
    serverAddress.sin_port = htons(portNumber);
    //and try to stablish a connection 
    if (connect(socketFileDescr, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
    {
        error("ERROR connecting");
    }
    else{
        printf("Connection OK...\n");
    }
    //if the connection are stablished
    //start to read the file specified in console
    ifstream fileToRead;
    cout<<"File to Open: ";
    cout<< argv[3]<<endl;
    fileToRead.open(argv[3]);
    char output[256];
    if (fileToRead.is_open())
    {
        printf("File Opened...\n");
        //and concatenate every line in a string variable
        //(i really need tofinda better way to serialize data to send via sockets)
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
    //close the file
    fileToRead.close();
    printf("File Closed...\n");
    
    //and write the file information in the socket.
    n = write(socketFileDescr, result.data(), result.capacity());
    
    if (n < 0){
        error("ERROR writing to socket");
    }
    else{
        cout << n+" bytes are sent to server..."<<endl;
    }
    //no, just wait for server response    
    bzero(buffer, 256);
    n = read(socketFileDescr, buffer, 255);
    if (n < 0){
        error("ERROR reading from socket");
    }
    //and print it.
    printf("%s\n", buffer);
    close(socketFileDescr);
    return 0;
}