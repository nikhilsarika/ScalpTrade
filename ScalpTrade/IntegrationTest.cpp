#include <iostream>
#include <string>

#include "MessageHeader.h"
#include "Quote.h"
#include "Trade.h"
#include "Order.h"
#include "TestVariables.h"
#include "SocketServer.h"
#include <queue>
#include <thread>
using std::string;
using std::thread;
//Concurrent Queue defined to Push Byte data to create random input values
ConcurrentQueue<char*> *testQueue = new ConcurrentQueue<char*>();

void intergrationTestSendData(string tcpIp, string tcpPort);
void intergrationTestReceiveData(string tcpIp, string tcpPort);
int main() {

	intergrationTestReceiveData("127.0.0.1","15000");
	intergrationTestSendData("127.0.0.1","14000");
	

	return 0;
}

void intergrationTestReceiveData(string tcpIp, string tcpPort){
	std::cout << "Before Socket Object Creation: " << '\n';
	SocketServer* socketServer = new SocketServer(tcpIp, tcpPort);
	std::cout << "after Socket Object Creation: " << '\n';
	thread receiveDataThread(&SocketServer::receiveMessages,socketServer);
	receiveDataThread.detach();
}

void intergrationTestSendData(string tcpIp, string tcpPort){
         
	//Test Data Generation to Stream to the 
	MessageHeader* header = new MessageHeader(9, 1);
	Quote* quote = new Quote(*header, "IBM", 1580152659000000000ULL, 5, 13897, 9, 13898);

	MessageHeader* header1 = new MessageHeader(9, 2);
	Trade* trade = new Trade(*header1, "IBM", 1580152659000000000ULL, 100, 13900);

	auto endofVWapWindow = std::chrono::system_clock::now() + std::chrono::seconds(3);
	while (std::chrono::system_clock::now() < endofVWapWindow) {
		//char* quotePointer = reinterpret_cast<char*>(quote);
		char* tradePointer = reinterpret_cast<char*>(trade);
		//testQueue->push(quotePointer);
		testQueue->push(tradePointer);

	}

	endofVWapWindow = std::chrono::system_clock::now() + std::chrono::seconds(3);
	while (std::chrono::system_clock::now() < endofVWapWindow) {
		char* quotePointer = reinterpret_cast<char*>(quote);
		testQueue->push(quotePointer);
	}


	std::cout << "Before Socket Object Creation: " << '\n';
	SocketServer* socketServer = new SocketServer(tcpIp, tcpPort);
	std::cout << "after Socket Object Creation: " << '\n';
	socketServer->sendMessages();
	// thread sendDataThread(&SocketServer::sendMessages,socketServer);
	// sendDataThread.detach();

}