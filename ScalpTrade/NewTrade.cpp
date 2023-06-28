#include "NewTrade.h"
using std::string;

NewTrade::NewTrade( string symbol, uint64_t timestamp, uint32_t quantity, int32_t price) :
	 symbol(symbol), timestamp(timestamp), quantity(quantity), price(price)
{

}

NewTrade::~NewTrade() {}