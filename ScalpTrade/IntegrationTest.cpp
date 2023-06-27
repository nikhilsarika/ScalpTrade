#include <iostream>
#include <string>

#include "MessageHeader.h"
#include "Quote.h"
#include "Trade.h"
#include "Order.h"
#include "TestVariables.h"
#include "SocketServer.h"

using std::string;

//Concurrent Queue defined to Push Byte data to create random input values
ConcurrentQueue<char*> *testQueue = new ConcurrentQueue<char*>();

void intergrationTest(string tcpIp, string tcpPort);
int main() {

	intergrationTest("127.0.0.1","14000");

	return 0;
}

void intergrationTest(string tcpIp, string tcpPort){

	//Test Data Generation to Stream to the 
	MessageHeader* header = new MessageHeader(9, 1);
	Quote* quote = new Quote(*header, "IBM", 1580152659000000000ULL, 5, 13897, 9, 13898);

	MessageHeader* header1 = new MessageHeader(9, 2);
	Trade* trade = new Trade(*header1, "IBM", 1580152659000000000ULL, 100, 13900);

	auto endofVWapWindow = std::chrono::system_clock::now() + std::chrono::seconds(3);
	bool tempFlag = true;
	while (std::chrono::system_clock::now() < endofVWapWindow) {
	//while (tempFlag) {
		unsigned char* check = NULL;
		char* quotePointer = reinterpret_cast<char*>(quote);
		char* TradePointer = reinterpret_cast<char*>(trade);
		testQueue->push(quotePointer);
		testQueue->push(TradePointer);
		tempFlag = false;

	}

	// int count = 30;
	// while (count >0) {
	// //while (tempFlag) {
	// 	unsigned char* check = NULL;
	// 	char* quotePointer = reinterpret_cast<char*>(quote);
	// 	char* TradePointer = reinterpret_cast<char*>(trade);
	// 	testQueue->push(quotePointer);
	// 	testQueue->push(TradePointer);
	// 	count--;

	// }
	std::cout << "Before Socket Object Creation: " << '\n';
	SocketServer* socketServer = new SocketServer(tcpIp, tcpPort);
	std::cout << "after Socket Object Creation: " << '\n';
    socketServer->sendMessages();

}