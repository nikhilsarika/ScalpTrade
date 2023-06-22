#include "Quote.h"
using std::string;

Quote::Quote(MessageHeader* header, string symbol, uint64_t timestamp, uint32_t bidQuantity, uint32_t bidPrice, uint32_t askQuantity, int32_t askPrice) :
	header(header), symbol(symbol), timestamp(timestamp), bidQuantity(bidQuantity), bidPrice(bidPrice), askQuantity(askQuantity), askPrice(askPrice)
{

}

Quote::~Quote() {}