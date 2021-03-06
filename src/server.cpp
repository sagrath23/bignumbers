#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <fstream>
#include <strings.h>
#include <stdlib.h>
#include <string>
#include <pthread.h>
#include <vector>

#include "bignumber/bignumber.h"

using namespace std;

/*
Function to print in prompt each position of the input array
*/
void printArray(vector<BigNumber> arr)
{
    for (unsigned i = 0; i < arr.size(); i++)
    {
        cout << arr[i].str() + "\n" <<endl;
    }
}

/*
Function to do the merge of two subarrays, attempting to order the array
*/
void merge(vector<BigNumber> &arr, int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    /* create temp arrays */
    vector<BigNumber> L(n1,0ul), R(n2,0ul);
    //BigNumber L[n1], R[n2];

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
void mergeSort(vector<BigNumber> &arr, int l, int r)
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

//Prototype function to allow multithreading
void *task1(void *);

//socket file definition to income conection
static int connFd;

int main(int argc, char* argv[])
{
    //define portNumber and socket listener file definition
    int portNo, listenFd;
    struct sockaddr_in svrAdd, clntAdd;//server and client addresses
    
    pthread_t threadA[3]; //threads that server can handle
    
    //first, check the input parameter
    if (argc < 2)
    {
        cerr << "Syntam : ./server <port>" << endl;
        return 0;
    }
    
    portNo = atoi(argv[1]);//and convert the port specified in integer
    //check if this port are in a safe range
    if((portNo > 65535) || (portNo < 2000) ||(portNo == 80))
    {
        cerr << "Please enter a port number between 2000 - 65535 or 80" << endl;
        return 0;
    }
    
    //create socket
    listenFd = socket(AF_INET, SOCK_STREAM, 0);
    
    if(listenFd < 0)
    {
        cerr << "Cannot open socket" << endl;
        return 0;
    }
    //clear svrAdd
    bzero((char*) &svrAdd, sizeof(svrAdd));
    //and set new values
    svrAdd.sin_family = AF_INET;
    svrAdd.sin_addr.s_addr = INADDR_ANY;
    svrAdd.sin_port = htons(portNo);
    
    //bind socket
    if(bind(listenFd, (struct sockaddr *)&svrAdd, sizeof(svrAdd)) < 0)
    {
        cerr << "Cannot bind" << endl;
        return 0;
    }
    //and set to listen state
    listen(listenFd, 5);
    
    int noThread = 0;

    while (noThread < 3)
    {
        cout << "Listening" << endl;

        socklen_t  len = sizeof(clntAdd);//store size of the address

        //this is where client connects. svr will hang in this mode until client conn
        connFd = accept(listenFd, (struct sockaddr *)&clntAdd, &len);

        if (connFd < 0)
        {
            cerr << "Cannot accept connection" << endl;
            return 0;
        }
        else
        {
            cout << "Connection successful" << endl;
        }
        
        pthread_create(&threadA[noThread], NULL, task1, NULL); 
        
        noThread++;
    }
    
    for(int i = 0; i < 3; i++)
    {
        pthread_join(threadA[i], NULL);
    }
    
    
}

/*
Task to be executed by a thread
*/
void *task1 (void *dummyPt)
{
    cout << "Thread No: " << pthread_self() << endl;
    char test[300];
    vector<BigNumber> numbers;
    bzero(test, 301);
    bool loop = false;
    while(!loop)
    {       
        bzero(test, 301);

        read(connFd, test, 300);
        
        string tester (test);
        if(tester == "process"){
            break; 
        }
        cout << tester << endl;
        BigNumber number(tester);
        numbers.push_back(number);
    }
    //proceed to order the array
    cout << "unordered" << endl;
    printArray(numbers);
    mergeSort(numbers, 0, numbers.size()-1);
    cout << "ordered" << endl;
    printArray(numbers);
    cout << "sum elements..." << endl;
    BigNumber acm;
    for(unsigned i = 0; i < numbers.size(); i++){
        acm += numbers[i];
    }
    cout << "result" << endl;
    cout << acm << endl;

    //now, send data back to user
    
    
    for(unsigned i = 0; i < numbers.size(); i++){
        write(connFd, numbers[i].str().data(), strlen(numbers[i].str().data()));    
    }
    //finally, send the result of the sum
    write(connFd, acm.str().data(), strlen(acm.str().data()));    
    //and finish connection
    write(connFd, "exit", strlen("exit"));
    cout << "\nClosing thread and conn" << endl;
    close(connFd);
    return 0;
}