#pragma once
#include <string>
#include <cstdint>
class NewQuote 
{
public:
	std::string symbol;
	uint64_t timestamp;
	uint32_t bidQuantity;
	uint32_t bidPrice;
	uint32_t askQuantity;
	int32_t askPrice;
	NewQuote() = default;
	NewQuote(std::string symbol, uint64_t timestamp, uint32_t bidQuantity, uint32_t bidPrice, uint32_t askQuantity, int32_t askPrice);
	~NewQuote();

};