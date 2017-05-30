#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <vector>
#include "bignumber/bignumber.h"

using namespace std;
/*
Function to display controlled errors
*/
void error(const char *msg)
{
    perror(msg);
    exit(1);
}

/*
Function to do the merge of two subarrays, attempting to order the array
*/
void merge(BigNumber arr[], int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    /* create temp arrays */
    BigNumber L[n1], R[n2];

    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    /* Merge the temp arrays back into arr[l..r]*/
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = l; // Initial index of merged subarray
    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            arr[k] = L[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    /* Copy the remaining elements of L[], if there
       are any */
    while (i < n1)
    {
        arr[k] = L[i];
        i++;
        k++;
    }

    /* Copy the remaining elements of R[], if there
       are any */
    while (j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
    }
}

/*
Function that subdivide input array and try to merge each subdivision
Using "Divide & Conquer" Strategy to order the input array
*/
void mergeSort(BigNumber arr[], int l, int r)
{
    if (l < r)
    {
        // Same as (l+r)/2, but avoids overflow for
        // large l and h
        int m = l + (r - l) / 2;

        // Sort first and second halves
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);

        merge(arr, l, m, r);
    }
}

/*
Function to print in prompt each position of the input array
*/
void printArray(BigNumber arr[])
{
    for (int i = 0; i < 6; i++)
    {
        cout << arr[i].str() + "\n" <<endl;
    }
}

void doStuff(int); /* function prototype to handle every socket connection in a new process */

int main(int argc, char *argv[])
{
    //like a client, is necessary to define:
    int socketFileDesc;// a socket fileDescriptor
    int newSocketFileDesc;// a new socket fileDescriptor, to handle a client's request
    int portNumber;//a port number to listen all request
    int processId;//and a process id related to that request
    socklen_t sizeClientAddr;//a type definition to handle client address size and length
    struct sockaddr_in serverAddress;//an structure to handle the server's address
    struct sockaddr_in clientAddress;//an structure to handle the client's address

    //to start the server, are validated the number of arguments introduced in the prompt
    if (argc < 2)// ./server 8080
    {
        fprintf(stderr, "ERROR, no port provided\n");
        exit(1);
    }
    else{
        printf("Arguments OK...\n");
    }
    socketFileDesc = socket(AF_INET, SOCK_STREAM, 0);//create a socket that use ip v.4 to stream data 
    if (socketFileDesc < 0){//validate the creation of the socket
        error("ERROR opening socket");
    }
    else{
        printf("Socket opened...\n");
    }
    //now, erase the data reated to server address
    bzero((char *)&serverAddress, sizeof(serverAddress));
    portNumber = atoi(argv[1]);//convert to int the port number
    //and set it again defining it's values 
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(portNumber);
    //now, try to bind address values to socket 
    if (bind(socketFileDesc, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0){
        error("ERROR on binding");
    }
    else{
        printf("Binding correct...\n");
    }
    //if binding socket is successful, mark as connection-mode socket 
    //and it's ready to listen requests    
    listen(socketFileDesc, 5);
    sizeClientAddr = sizeof(clientAddress);
    while (1)
    {
        //when a new connection request is listened, the server try to accept it
        newSocketFileDesc = accept(socketFileDesc, (struct sockaddr *)&clientAddress, &sizeClientAddr);
        if (newSocketFileDesc < 0){
            error("ERROR on accept");
        }
        else{
            printf("Connection accepted...\n");
        }
        //if is accepted, then generate a new process to attend it    
        processId = fork();
        if (processId < 0){
            error("ERROR on fork");
        }
        else{
            printf("New connection process started...\n");
        }
        //and call the function that do all the stuffs    
        if (processId == 0)
        {
            close(socketFileDesc);
            doStuff(newSocketFileDesc);
            exit(0);
        }
        else
            close(newSocketFileDesc);
    }
    close(socketFileDesc);
    return 0; /* we never get here */
}

/*
 There is a separate instance of this function 
 for each connection.  It handles all communication
 once a connnection has been established.
 */
void doStuff(int sock)
{
    int n;//number of bytes readed from buffer
    char buffer[256];//buffer

    vector<BigNumber> numbers;

    bzero(buffer, 256);
    n = read(sock, buffer, 255);
    if (n < 0){
        error("ERROR reading from socket");
    }
    else{
        printf("Socket readed OK\n");
    }
    //substract from buffer 
    char * pch;
    pch = strtok (buffer,"/");
    while (pch != NULL)
    {
        printf ("%s\n",pch);
        //add it to a vector
        string temp;
        temp = string(pch);
        BigNumber tempNumber(temp);
        numbers.push_back(tempNumber);
        pch = strtok (NULL, "/");
    }

    cout<< numbers.size() << " numbers readed..."<<endl;

    n = write(sock, "I got your message", 18);

    /*
    string result;
    result = string(buffer);

    
    string number = "12345678998765432112345678900";
    string number2 = "20000000000000000000000000001";
    string number3 = "30000000000000000000000000001";
    string number4 = "40000000000000000000000000001";
    string number5 = "50000000000000000000000000001";

    BigNumber testNumber(number);
    BigNumber testNumber2(number2);
    BigNumber testNumber3(number3);
    BigNumber testNumber4(number4);
    BigNumber testNumber5(number5);
    BigNumber testNumber6(number2);

    if (testNumber > testNumber2)
    {
        printf("testNumber is greater\n");
    }
    else
    {
        printf("testNumber2 is greater\n");
    }

    //array<BigNumber, 5> numbers = {testNumber3, testNumber5, testNumber, testNumber4, testNumber2, testNumber6};
    BigNumber numbers[6] = {testNumber3, testNumber5, testNumber, testNumber4, testNumber2, testNumber6};
    printf("unordered\n");
    printArray(numbers);
    //order algorithm
    mergeSort(numbers, 0, 5);
    printf("ordered\n");
    printArray(numbers);
    //BigNumber numbers[3] = {testNumber, testNumber2, testNumber3};

    //ServerTasks tasks;
    BigNumber acm;
    for (int i = 0; i < 10000; i++)
    {
        acm = testNumber + testNumber2 + testNumber3;
    }
    */
    //string result;
    //result = acm.str();
    //printf((result).c_str());
    printf("\n");
    if (n < 0)
        error("ERROR writing to socket");
}