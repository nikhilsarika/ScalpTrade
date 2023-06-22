#include "Trade.h"
using std::string;

Trade::Trade(MessageHeader* header, string symbol, uint64_t timestamp, uint32_t quantity, int32_t price) :
	header(header), symbol(symbol), timestamp(timestamp), quantity(quantity), price(price)
{

}

Trade::~Trade() {}