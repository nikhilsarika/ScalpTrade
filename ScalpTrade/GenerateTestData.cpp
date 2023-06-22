//#include <iostream>
//#include <string>
//#include <cstdlib>
//#include "MessageHeader.h"
//#include "Quote.h"
//#include "Trade.h"
//#include "Order.h"
//
//
//#include <array>
//#include <memory>
//#include <type_traits>
//#include <iomanip>
//
//using std::string;
//using std::atoi;
//using std::cout;
//
//using byte = unsigned char;
//template< typename T > std::array< byte, sizeof(T) >  to_bytes(const T& object);
//int main(int argc, char* argv[]) {
//
//	MessageHeader* header = new MessageHeader(9, 1);
//	Quote* quote = new Quote(header, "IBM", 1580152659000000000ULL, 5, 13897, 9, 13898);
//	Trade* trade = new Trade(header, "IBM", 1580152659000000000ULL, 100, 13900);
//
//
//	const auto bytes = to_bytes(*quote);
//	std::cout << std::hex << std::setfill('0');
//	for (byte b : bytes) std::cout << std::setw(2) << int(b) << ' ';
//	std::cout << '\n';
//
//
//
//	return 0;
//}
//
//
//template< typename T > std::array< byte, sizeof(T) >  to_bytes(const T& object)
//{
//	std::array< byte, sizeof(T) > bytes;
//
//	const byte* begin = reinterpret_cast<const byte*>(std::addressof(object));
//	const byte* end = begin + sizeof(T);
//	std::copy(begin, end, std::begin(bytes));
//
//	return bytes;
//}