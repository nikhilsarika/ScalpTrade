#include <iostream>
#include <string>
#include <cstdlib>
#include "MessageHeader.h"
#include "Quote.h"
#include "Trade.h"
#include "Order.h"
#include "ConcurrentQueue.h"
#include <chrono>
#include <algorithm>
#include <thread>
#include <atomic>
#include "global.h"
#include "SocketClient.h"
#include "SocketServer.h"
#include "NewTrade.h"
#include "NewQuote.h"



using std::string;
using std::atoi;
using std::cout;
using std::max;
using std::thread;
using std::array;
using byte = unsigned char;


//Global Variables to used across multiple Threads
std::atomic<long long int> vwap_numerator(0);
std::atomic<long long int> vwap_denominator(0);
std::atomic<long long int> vwap(0);
std::atomic<bool> initialVwapCalculated(false);
std::atomic<bool> triggerVwapUpdate(true);


//Trade* newTrade = new Trade();
char* outputBytes = nullptr;
Trade* newTrade = nullptr; 

//Concurrent Queues defined to Consume Byte data from the Market Data TCP Streams.
ConcurrentQueue<Trade>* tradeQueue = new ConcurrentQueue<Trade>();

//Concurrent Queue defined to Push Byte data to the input market TCP Stream.
ConcurrentQueue<char*> *inputByteQueue = new ConcurrentQueue<char*>();

//Concurrent Queue defined to Push Byte data to the Output Order TCP Stream.
ConcurrentQueue<char*> *outputByteQueue = new ConcurrentQueue<char*>();


//Concurrent Queue defined to Push Byte data to create random input values
ConcurrentQueue<char*> *testQueue = new ConcurrentQueue<char*>();

//Methods declared to implement the business logic of the Application
void processQuote(NewQuote* newQuote, string const& orderSide, int const& maxQuantity);
void processTrade(Trade* newTrade, int const& vwapWindow);
void placeNewOrder(string symbol, char side, uint32_t quantity, int32_t price);
void updateVwap(int const& vwapWindow);
void intergrationTest(string tcpIp, string tcpPort);


//Driver Function to read Start the Application and Read Command Line Inputs
int main(int argc, char* argv[]) {
	

	//Variables Defined to Store the Command Line inputs
	const string symbol= argv[1];
	const string orderSide = argv[2];
	const int maxOrderSize = atoi(argv[3]);
	const int vwapWindow = atoi(argv[4]);
	const string marketTcpIP = argv[5];
	const string marketTcpPort = argv[6];
	const string orderTcpIP = argv[7];
	const string orderTcpPort = argv[8];

	cout << "Symbol : " << symbol << '\n';
	cout << "orderSide : " << orderSide << '\n';
	cout << "maxOrderSize : " << maxOrderSize << '\n';
	cout << "vwapWindow : " << vwapWindow << '\n';
	cout << "marketTcpIP : " << marketTcpIP << '\n';
	cout << "marketTcpPort : " << marketTcpPort << '\n';
	cout << "orderTcpIP : " << orderTcpIP << '\n';
	cout << "orderTcpPort : " << orderTcpPort << '\n';

	
	SocketClient* marketClient = new SocketClient(marketTcpIP,marketTcpPort);

	thread marketCleintThread(&SocketClient::startReceivingData,marketClient);
	marketCleintThread.detach();

	SocketClient* orderClient = new SocketClient(orderTcpIP,orderTcpPort);

	thread orderClientThread(&SocketClient::startReceivingData,orderClient);
	orderClientThread.detach();

	//Messages are read from the messageQueue that contains the messages received from Sockets
	// auto endofVWapWindow = std::chrono::system_clock::now() + std::chrono::seconds(3);
	// while (std::chrono::system_clock::now() < endofVWapWindow) {
	std::cout << "Before while" << '\n';
	while (true) {
		// std::cout<< "Inside Infinite While loop : " << inputByteQueue->isEmpty() << '\n';
	// while (!byteQueue->isEmpty()) {
		if(!inputByteQueue->isEmpty()){
			
			char* currentByteStream = inputByteQueue->pop();
			MessageHeader* currentHeader = reinterpret_cast<MessageHeader*>(currentByteStream);
			//MessageHeader currentHeader = trade->header;
			std::cout<< "Order type from header: " << static_cast<int>(currentHeader->type)  << '\n';
			std::cout<< "Order leght from header: " << static_cast<int>(currentHeader->length)  << '\n';



			//Messages are processed Based on the Message Header Type i.e. 1 for Quote, 2 for Trade and Exception for all other types
			if (initialVwapCalculated && static_cast<int>(currentHeader->type) == 1) {
				std::cout<< "Inside the Quote of logic : " << inputByteQueue->isEmpty() << '\n';
				currentByteStream  = currentByteStream + sizeof(currentHeader);
				NewQuote* newQuote = reinterpret_cast<NewQuote*>(currentByteStream);
				processQuote(newQuote, orderSide, maxOrderSize);
			}
			else if (static_cast<int>(currentHeader->type) == 2){
				//std::cout<< "Inside the Trade of logic : " << inputByteQueue->isEmpty() << '\n';
				currentByteStream = currentByteStream + sizeof(currentHeader);
				NewTrade* currTrade = reinterpret_cast<NewTrade*>(currentByteStream);
				cout<< "Before Process Trade" << '\n';
				Trade*  trade = new Trade(*currentHeader,currTrade->symbol, currTrade->timestamp,currTrade->quantity,currTrade->price);
				processTrade(trade, vwapWindow);
				cout<< "After Process Trade" << '\n';
			}
			else {
				std::cout<< "Inside the else of logic : " << inputByteQueue->isEmpty() << '\n';
				//Throw Exception as Invalid Message Type is received. 
			}
		
		}

	}
	   
	return 0;
}


//Quote messages are processed based on the order side and Vwap Price
void processQuote(NewQuote* newQuote, string const& orderSide, int const& maxQuantity) {
	// std::cout<< "Bid Price : " << static_cast<int>(newQuote->bidPrice) << '\n';
	// std::cout<< "Calculated Vwap: " << vwap<< '\n';
	// std::cout<< "Order Side: " << orderSide << '\n';
	// std::cout<< "Condition 1 : " << (orderSide == "B" && static_cast<int>(newQuote->bidPrice) < vwap)  << '\n';
	// std::cout<< "Condition 2 : " << (orderSide == "S" && static_cast<int>(newQuote->askPrice) > vwap) << '\n';
	if (orderSide == "B" && static_cast<int>(newQuote->bidPrice) < vwap) {
		placeNewOrder(newQuote->symbol, 'B', max(maxQuantity, static_cast<int>(newQuote->bidQuantity)), newQuote->bidPrice);
	}
	else if (orderSide == "S" && static_cast<int>(newQuote->askPrice) > vwap) {
		std::cout<< "Place Order: " << orderSide << '\n';
		placeNewOrder(newQuote->symbol, 'S', max(maxQuantity, static_cast<int>(newQuote->askQuantity)), newQuote->askPrice);
	}
	else {
		//Log a message saying no order is Placed. 
	}
}

//Trade messages are Pushed onto a Queue and Asychronously processed to calculate Vwap based on the vwapWindow, Trade Price and Trade Quantity
void processTrade(Trade* newTrade,int const& vwapWindow) {

	tradeQueue->push(*newTrade);
	if (triggerVwapUpdate) {
		//vwap is updated in the background as the trade messages are pushed on the Trade Queue. 
		thread vwapThread(updateVwap, vwapWindow);
		vwapThread.detach();
		triggerVwapUpdate = false;
	}
}

//Method to calculate Vwap value based on the received input Trade messages
void updateVwap(int const& vwapWindow) {
	// std::cout<< "Inside update vwap : " << vwapWindow << '\n';
	// std::cout<< "Inside update initialVwapCalculated : " << initialVwapCalculated << '\n';
	// std::cout<< "Inside update triggerVwapUpdate : " << triggerVwapUpdate << '\n';
	//Caluate Time window based on the input
	Trade trade;
	//std::cout<< "Before DO WHile " << triggerVwapUpdate << '\n';
	auto endofVWapWindow = std::chrono::system_clock::now() + std::chrono::seconds(vwapWindow);

	//Code to calcualate vwap needs to be triggered atleast one time based on the given requirements
	//tradeQueue->isEmpty() not used to end the loop as the loop has to be kept alive for the given window irrespective of availability of Trade data  in the queue
	do {
		if (!tradeQueue->isEmpty()) {
			trade = tradeQueue->pop();
			// std::cout<< "Inside trade.price : " << trade.price << '\n';
			// std::cout<< "Inside utrade.quantity: " << trade.quantity << '\n';
			vwap_numerator = vwap_numerator + (trade.price * trade.quantity);
			vwap_denominator = vwap_denominator + trade.quantity;
		}
	} while (std::chrono::system_clock::now() < endofVWapWindow);
	
	//No zero check for denominator as its assumed that trade quantity would not be zero.
	vwap = vwap_numerator / vwap_denominator;

	std::cout<< "Calculated VWAP: " << vwap << '\n';
	

	//Set the values to facilitate the next vwap calculation
	initialVwapCalculated = true;
	triggerVwapUpdate = true;
	vwap_numerator = 0;
	vwap_denominator = 0;
}

//Function defined to create a new order and push it to that order queue that is utiized by the Sockets to push data to Order TCP Stream
void placeNewOrder(string symbol, char side, uint32_t quantity, int32_t price) {


	cout << "inside place order" << '\n';
	uint64_t timestamp = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	Order* newOrder = new Order(symbol, timestamp, side, quantity, price);
	
	outputBytes = reinterpret_cast<char*>(newOrder);
	outputByteQueue->push(outputBytes);

}

