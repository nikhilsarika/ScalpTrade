#include "Order.h"
using std::string;

Order::Order( string symbol, uint64_t timestamp, char side, uint32_t quantity, int32_t price) :
	 symbol(symbol), timestamp(timestamp), side(side), quantity(quantity), price(price)
{

}

Order::~Order(){}