# ScalpTrade
	
ScalpTrade.cpp is Entry point to the Program Solution

## Version

* C++ : 14

## Steps To Run Executable

* Run the make file in the Source Direcotry using below Command 

```bash
make
```

* Run the Executable file created by Make using the below command

```bash
./ScalpTrade IBM B 100 1 127.0.0.1 14000 127.0.0.1 15000 
```

# Data Structures Used
* Concurrent Queue - Queue with Mutex locks

# Threads Used
* Thread to start the Socket Client to receive Market Data in Deatached mode.
* Thread to start a socket client to send Order Data in Detached mode.
* Thread to asynchronously calculate VWAP value.



## Design 

* Class files are defined for Quote, Trade, Order and Message Headers.
* Command line inputs are stored in variables and utilized in method calls.
* A generic Socket client class is implemented to support both sending and receving of messages. 
* The Socket Clients are initialized with port, ip and respective methods are invoked asynchronously using a detached thread
* Data is popped from the queues and Reinterpret Cast is used to retrieve the respective objects

## Assumptions

* Implemented Has a Relationship to get specific bytes for each message type
* Assumed only two message types will be received in the input
* Utillized a do while loop as the Vwap value needs to be caluated only if atlease on value is present. 
* Quantity is assumed to be a non zero value. 
* Return value from read() method in sockets is used to Check if Socket connection is alive.

## Integration Testing

* Intergration test is implemetned to simulate a local socket server to send and receive messages
* A seperate make file is created facilitate integration testing.

### Steps to run the test

* Run the integration test makefile
```bash
make -f makefileTest
```
* Execute the SocketServer executable created in the above step using the below command
```bash
./StartSocketServer
```


## Refrences

Concurrent Queue: https://www.geeksforgeeks.org/implement-thread-safe-queue-in-c/

Socket ref: https://www.cs.dartmouth.edu/~campbell/cs50/socketprogramming.html

Templated Class: https://bytefreaks.net/programming-2/c/c-undefined-reference-to-templated-class-function

MultiThreading: https://stackoverflow.com/questions/73007186/threads-calling-function-in-another-class





