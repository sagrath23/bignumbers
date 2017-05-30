#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "bignumber/bignumber.h"

using namespace std;

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

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

/* l is for left index and r is right index of the
   sub-array of arr to be sorted */
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

void printArray(BigNumber arr[])
{
    for (int i = 0; i < 6; i++)
    {
        cout << arr[i].str() + "\n" <<endl;
    }
}

void doStuff(int); /* function prototype */

int main(int argc, char *argv[])
{
    int socketFileDesc, newSocketFileDesc, portNumber, processId;
    socklen_t sizeClientAddr;
    struct sockaddr_in serverAddress, clientAddress;

    if (argc < 2)
    {
        fprintf(stderr, "ERROR, no port provided\n");
        exit(1);
    }
    else{
        printf("Arguments OK...\n");
    }
    socketFileDesc = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFileDesc < 0){
        error("ERROR opening socket");
    }
    else{
        printf("Socket opened...\n");
    }
    bzero((char *)&serverAddress, sizeof(serverAddress));
    portNumber = atoi(argv[1]);
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(portNumber);
    if (bind(socketFileDesc, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0){
        error("ERROR on binding");
    }
    else{
        printf("Binding correct...\n");
    }
        
    listen(socketFileDesc, 5);
    sizeClientAddr = sizeof(clientAddress);
    while (1)
    {
        newSocketFileDesc = accept(socketFileDesc, (struct sockaddr *)&clientAddress, &sizeClientAddr);
        if (newSocketFileDesc < 0){
            error("ERROR on accept");
        }
        else{
            printf("Connection accepted...\n");
        }
            
        processId = fork();
        if (processId < 0){
            error("ERROR on fork");
        }
        else{
            printf("New connection process started...\n");
        }
            
        if (processId == 0)
        {
            close(socketFileDesc);
            doStuff(newSocketFileDesc);
            exit(0);
        }
        else
            close(newSocketFileDesc);
    } /* end of while */
    close(socketFileDesc);
    return 0; /* we never get here */
}

/******** doStuff() *********************
 There is a separate instance of this function 
 for each connection.  It handles all communication
 once a connnection has been established.
 *****************************************/
void doStuff(int sock)
{
    int n;
    char buffer[256];

    bzero(buffer, 256);
    n = read(sock, buffer, 255);
    if (n < 0){
        error("ERROR reading from socket");
    }
    else{
        printf("Socket readed OK\n");
    }
        
    //printf("Here is the message: %s\n", buffer);
    char * pch;
    pch = strtok (buffer,"/");
    while (pch != NULL)
    {
        printf ("%s\n",pch);
        //add it to a vector
        
        pch = strtok (NULL, " ,.-");
    }

    string result;
    result = string(buffer);

    n = write(sock, "I got your message", 18);
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
    string result;
    result = acm.str();
    //printf((result).c_str());
    printf("\n");
    if (n < 0)
        error("ERROR writing to socket");
}