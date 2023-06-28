#include <iostream>
#include <string>

#include "MessageHeader.h"
#include "Quote.h"
#include "Trade.h"
#include "Order.h"
#include "TestVariables.h"
#include "SocketServer.h"
#include <queue>
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
	Trade* trade = new Trade(*header1, "IBM", 1580152659000000000ULL, 98, 13901);

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

	// int count = 30;
	// while (count >0) {
	// //while (tempFlag) {
	// 	char* tradePointer = reinterpret_cast<char*>(trade);
	// 	testQueue->push(tradePointer);
	// 	count--;

	// }

	// while(!testQueue->isEmpty()){
	
	// 	char* currentByteStream = testQueue->pop();
	// 	MessageHeader* currentHeader = reinterpret_cast<MessageHeader*>(currentByteStream);
	// 	Trade* trade = reinterpret_cast<Trade*>(currentByteStream);
	// 	std::cout<< "Order type from header: " << static_cast<int>(currentHeader->type)  << '\n';
	// 	std::cout<< "Order leght from header: " << static_cast<int>(currentHeader->length)  << '\n';

	// 	std::cout<< "Price"<<trade->price<< '\n';
	// 	std::cout<< "Quantity"<<trade->quantity<< '\n';

	// 	std::cout<< "Symbol"<<trade->symbol<< '\n';
	// 	std::cout<< "Timestamp"<<trade->timestamp<< '\n';
	// }

	// count = 30;
	// while (count >0) {
	// 	char* quotePointer = reinterpret_cast<char*>(quote);
	// 	testQueue->push(quotePointer);
	// 	count--;

	// }


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