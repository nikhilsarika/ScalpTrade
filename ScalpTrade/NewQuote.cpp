#include "NewQuote.h"
using std::string;

NewQuote::NewQuote( string symbol, uint64_t timestamp, uint32_t bidQuantity, uint32_t bidPrice, uint32_t askQuantity, int32_t askPrice) :
	symbol(symbol), timestamp(timestamp), bidQuantity(bidQuantity), bidPrice(bidPrice), askQuantity(askQuantity), askPrice(askPrice)
{

}

NewQuote::~NewQuote() {}