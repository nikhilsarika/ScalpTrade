#include <iostream>
#include <string>
#include <cstdlib>
#include "MessageHeader.h"
#include "Quote.h"
#include "Trade.h"
#include "Order.h"


#include <array>
#include <memory>
#include <type_traits>
#include <iomanip>

using std::string;
using std::atoi;
using std::cout;
using byte = unsigned char;
template< typename T > std::array< byte, sizeof(T) >  to_bytes(const T& object);
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
	  
	MessageHeader* header = new MessageHeader(9, 1);
	Quote* quote = new Quote(header, "IBM", 1580152659000000000ULL, 5, 13897, 9, 13898);
	Trade* trade = new Trade(header, "IBM", 1580152659000000000ULL, 100, 13900);


	auto bytes = to_bytes(*quote);
	std::cout << std::hex << std::setfill('0');
	for (byte b : bytes) std::cout << std::setw(2) << int(b) << ' ';
	std::cout << '\n';

	Quote* newQuote = reinterpret_cast<Quote*>(&bytes);

	unsigned int length = newQuote->header->length;
	unsigned int type = newQuote->header->type;
	string quote_symbol = newQuote->symbol;
	unsigned long long int timestamp = newQuote->timestamp;
	unsigned int  bidQuantity = newQuote->bidQuantity;
	unsigned int  bidPrice = newQuote->bidPrice;
	unsigned int  askQuantity = newQuote->askQuantity;
	int askPrice = newQuote->askPrice;

	cout << length << "\n";
	cout << type << "\n";
	cout << quote_symbol << "\n";
	cout << std::dec << timestamp << "\n";
	cout << bidQuantity << "\n";
	cout << std::dec << bidPrice << "\n";
	cout << askQuantity << "\n";
	cout << std::dec << askPrice << "\n";

	return 0;
}


template< typename T > std::array< byte, sizeof(T) >  to_bytes(const T& object)
{
	std::array< byte, sizeof(T) > bytes;

	const byte* begin = reinterpret_cast<const byte*>(std::addressof(object));
	const byte* end = begin + sizeof(T);
	std::copy(begin, end, std::begin(bytes));

	return bytes;
}