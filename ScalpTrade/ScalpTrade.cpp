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
using byte = unsigned char;

std::atomic<int> vwap_numerator(0);
std::atomic<int> vwap_denominator(0);
std::atomic<int> vwap(0);
std::atomic<bool> triggerVwapUpdate(true);
std::atomic<bool> vwapFlag(true);
Quote* newQuote = nullptr;
Trade* newTrade = nullptr;
ConcurrentQueue<Trade> *tradeQueue = new ConcurrentQueue<Trade>();
void processMessage(byte* input, string const& orderSide, int const& vwapWindow, int const& maxQuantity);
void placeNewOrder(string symbol, char side, uint32_t quantity, int32_t price);
void updateVwap(int const& vwapWindow);
int main(int argc, char* argv[]) {
	

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
	

	//Test Data Generation to Stream to the 
	MessageHeader* header = new MessageHeader(9, 1);
	Quote* quote = new Quote(header, "IBM", 1580152659000000000ULL, 5, 13897, 9, 13898);
	Trade* trade = new Trade(header, "IBM", 1580152659000000000ULL, 100, 13900);


	//auto quoteBytes = to_bytes(*quote);
	//auto tradeBytes = to_bytes(*trade);

	//Quote* newQuote = reinterpret_cast<Quote*>(&quoteBytes);
	//unsigned int length = newQuote->header->length;
	//unsigned int type = newQuote->header->type;
	//string quote_symbol = newQuote->symbol;
	//unsigned long long int timestamp = newQuote->timestamp;
	//unsigned int  bidQuantity = newQuote->bidQuantity;
	//unsigned int  bidPrice = newQuote->bidPrice;
	//unsigned int  askQuantity = newQuote->askQuantity;
	//int askPrice = newQuote->askPrice;

	//cout << length << "\n";
	//cout << type << "\n";
	//cout << quote_symbol << "\n";
	//cout << std::dec << timestamp << "\n";
	//cout << bidQuantity << "\n";
	//cout << std::dec << bidPrice << "\n";
	//cout << askQuantity << "\n";
	//cout << std::dec << askPrice << "\n";

	//Implement Socket to receive Byte data;
	//processMessage(input, orderSide, vwapWindow, maxQuantity);
	//Below quoteBytes and Tradebytes are sent by the socket
	processMessage(reinterpret_cast<byte*>(&quote), orderSide, vwapWindow, maxOrderSize);
	processMessage(reinterpret_cast<byte*>(&trade), orderSide, vwapWindow, maxOrderSize);

	return 0;
}


//template< typename T > byte* to_bytes(const T& object)
//{ 
//	byte* begin = reinterpret_cast<byte*>(std::addressof(object));
//	return begin;
//}


void processMessage(byte* input, string const& orderSide, int const& vwapWindow, int const& maxQuantity) {

	MessageHeader* messageHeader = reinterpret_cast<MessageHeader*>(*input);

	if (vwapFlag && messageHeader->type == 1) {
		newQuote = reinterpret_cast<Quote*>(&input);
		if (orderSide == "B" && static_cast<int>(newQuote->bidPrice) < vwap) {
			placeNewOrder(newQuote->symbol, 'B', max(maxQuantity, static_cast<int>(newQuote->bidQuantity)), newQuote->bidPrice);
		}
		else if (orderSide == "S" && static_cast<int>(newQuote->askPrice) > vwap) {
			placeNewOrder(newQuote->symbol, 'S', max(maxQuantity, static_cast<int>(newQuote->askQuantity)), newQuote->askPrice);
		}
	}
	else if (messageHeader->type == 2) {
		newTrade = reinterpret_cast<Trade*>(&input);
		tradeQueue->push(*newTrade);
		if (triggerVwapUpdate) {
			//vwap is updated in the background as the trade messages are received. 
			thread vwapThread(updateVwap, vwapWindow);
			triggerVwapUpdate = false;
		}
	}
}


void updateVwap(int const& vwapWindow) {

	Trade trade = tradeQueue->pop();
	vwap_numerator = vwap_numerator + (trade.price * trade.quantity);
	vwap_denominator = vwap_denominator + trade.quantity;
	auto endofVWapWindow = std::chrono::system_clock::now() + std::chrono::seconds(vwapWindow);
	while (std::chrono::system_clock::now() < endofVWapWindow) {
		if (!tradeQueue->isEmpty()) {
			trade = tradeQueue->pop();
			vwap_numerator = vwap_numerator + (trade.price * trade.quantity);
			vwap_denominator = vwap_denominator + trade.quantity;
		}
	}
	vwap = vwap_numerator / vwap_numerator;
	vwapFlag = true;
	triggerVwapUpdate = true;
	vwap_numerator = 0;
	vwap_denominator = 0;
}


void placeNewOrder(string symbol, char side, uint32_t quantity, int32_t price) {
	uint64_t timestamp = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	Order* newOrder = new Order(symbol, timestamp, side, quantity, price);
	auto bytes = reinterpret_cast<byte*>(&newOrder);
	//add code to send new order. 
}