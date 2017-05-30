# bignumbers
Small Client/Server application written in C++ to handle really big numbers. 
The Client will read big really big numbers, and store them in an array, Then, pack and send them to
the server, which will execute some operations, in this order: 

1. Sort the array, using a non-bubble sort, (You will have to write this function/subroutine).
2. After sorting, it will have to sum the contents of the array.
3. finally, the resulting array, and the sum, will be transported back to the client.

To run this app, just clone this repository. Then, get into the folder and use make to compile and 
generate executables.

* Use make all to compile all sources of the app
* Use make client to compile only client source
* Use make server to compile only server source
* Use make number to compile only BigNumber source
* Use make clean to delete all executables

In order to execute correctly this app, first execute server app typing this command in console:
```sh
$ ./server <port>
```


Then, run client app, typing this command in another console:
```sh
$ ./client <host name> <port> <route_to_file>
```
