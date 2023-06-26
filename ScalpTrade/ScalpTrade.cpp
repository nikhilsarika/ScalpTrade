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

#include <array>
#include <memory>
#include <type_traits>
#include <iomanip>

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

//Variable to temporarily hold the Quote and Trade Pointers
Quote* newQuote = nullptr;
Trade* newTrade = nullptr;


//Concurrent Queues defined to Consume Byte data from the Market Data TCP Streams.
ConcurrentQueue<Trade> *tradeQueue = new ConcurrentQueue<Trade>();
ConcurrentQueue<MessageHeader*>* messageQueue = new ConcurrentQueue<MessageHeader*>();

//Concurrent Queue defined to Push Byte data to the Order TCP Stream.
ConcurrentQueue<Order>* orderQueue = new ConcurrentQueue<Order>();


//Methods Declared to process Quote and Trade Messages received from the Streams
void processQuote(Quote* newQuote, string const& orderSide, int const& maxQuantity);
void processTrade(Trade* newTrade, int const& vwapWindow);
void placeNewOrder(string symbol, char side, uint32_t quantity, int32_t price);
void updateVwap(int const& vwapWindow);
template< typename T > std::array< byte, sizeof(T) >  to_bytes(const T& object);

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

	//Test Data Generation to Stream to the 
	MessageHeader* header = new MessageHeader(9, 1);
	Quote* quote = new Quote(header, "IBM", 1580152659000000000ULL, 5, 13897, 9, 13898);

	MessageHeader* header1 = new MessageHeader(9, 2);
	Trade* trade = new Trade(header1, "IBM", 1580152659000000000ULL, 100, 13900);


	//Implement Socket to receive Byte data;
	//processMessage(input, orderSide, vwapWindow, maxQuantity);
	//Below quoteBytes and Tradebytes are sent by the socket
	//auto endofVWapWindow = std::chrono::system_clock::now() + std::chrono::seconds(3);
	bool tempFlag = true;
	//while (std::chrono::system_clock::now() < endofVWapWindow) {
	while (tempFlag) {
		messageQueue->push(reinterpret_cast<MessageHeader*>(quote));
		messageQueue->push(reinterpret_cast<MessageHeader*>(trade));
		tempFlag = false;
	}
	

	//Messages are read from the messageQueue that contains the messages received from Sockets
	while (!messageQueue->isEmpty()) {
		
		MessageHeader currentHeader = *messageQueue->pop();
		cout<<  "current header length " << currentHeader.length << '\n';
		//Messages are processed Based on the Message Header Type i.e. 1 for Quote, 2 for Trade and Exception for all other types
		if (initialVwapCalculated && currentHeader.type == 1) {
			newQuote = reinterpret_cast<Quote*>(&currentHeader);
			processQuote(quote, orderSide, maxOrderSize);
		}
		else if (currentHeader.type == 1){
			newTrade = reinterpret_cast<Trade*>(&currentHeader);
			processTrade(trade, vwapWindow);
		}
		else {
			//Throw Exception as Invalid Message Type is received. 
		}
		
	}
	   
	return 0;
}

//Need to be removed/moved after sockets are implemented
//Method defined to Create a byte array 
template< typename T > std::array< byte, sizeof(T) >  to_bytes(const T& object)
{
	std::array< byte, sizeof(T) > bytes;

	const byte* begin = reinterpret_cast<const byte*>(std::addressof(object));
	const byte* end = begin + sizeof(T);
	std::copy(begin, end, std::begin(bytes));

	return bytes;
}

//Quote messages are processed based on the order side and Vwap Price
void processQuote(Quote* newQuote, string const& orderSide, int const& maxQuantity) {
	if (orderSide == "B" && static_cast<int>(newQuote->bidPrice) < vwap) {
		placeNewOrder(newQuote->symbol, 'B', max(maxQuantity, static_cast<int>(newQuote->bidQuantity)), newQuote->bidPrice);
	}
	else if (orderSide == "S" && static_cast<int>(newQuote->askPrice) > vwap) {
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
	
	//Caluate Time window based on the input
	Trade trade;
	auto endofVWapWindow = std::chrono::system_clock::now() + std::chrono::seconds(vwapWindow);

	//Code to calcualate vwap needs to be triggered atleast one time based on the given requirements
	//tradeQueue->isEmpty() not used to end the loop as the loop has to be kept alive for the given window irrespective of availability of Trade data  in the queue
	do {
		if (!tradeQueue->isEmpty()) {
			trade = tradeQueue->pop();
			vwap_numerator = vwap_numerator + (trade.price * trade.quantity);
			vwap_denominator = vwap_denominator + trade.quantity;
		}
	} while (std::chrono::system_clock::now() < endofVWapWindow);
	

	//No zero check for denominator as its assumed that trade quantity would not be zero.
	vwap = vwap_numerator / vwap_denominator;


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
	
	orderQueue->push(*newOrder);

	//Temporary Logging - Need to be removed.
	auto bytes = to_bytes(&newOrder);
	std::cout << std::hex << std::setfill('0');
	for (byte b : bytes) std::cout << std::setw(2) << int(b) << ' ';
	std::cout << '\n';
}