# bignumbers
Small Client/Server application written in C++ to handle really big numbers. 
The Client will read big really big numbers, and store them in an array, Then, pack and send them to
the server, which will execute some operations, in this order: 

1. Sort the array, using a non-bubble sort, (You will have to write this function/subroutine).
2. After sorting, it will have to sum the contents of the array.
3. finally, the resulting array, and the sum, will be transported back to the client.
