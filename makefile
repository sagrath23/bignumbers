# the compiler: gcc for C program, define as g++ for C++
CC = g++
# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -g -Wall -pthread -lpthread
# the build target executable:
TARGET = src/
all: $(TARGET)
		$(CC) $(CFLAGS) -o client $(TARGET)client.cpp
		$(CC) $(CFLAGS) -o server $(TARGET)bignumber/bignumber.cpp $(TARGET)server.cpp
server: $(TARGET)
		$(CC) $(CFLAGS) -o server $(TARGET)bignumber/bignumber.cpp $(TARGET)server/server.cpp
client: $(TARGET)
		$(CC) $(CFLAGS) -o client $(TARGET)client.cpp
number: $(TARGET)
		$(CC) $(CFLAGS) $(TARGET)bignumber/bignumber.cpp 
clean:
	$(RM) client
	$(RM) server
