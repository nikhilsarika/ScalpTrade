#pragma once
#include <string>
#include <cstdint>
#include "MessageHeader.h"
class Quote 
{
public:
	MessageHeader header;
	std::string symbol;
	uint64_t timestamp;
	uint32_t bidQuantity;
	uint32_t bidPrice;
	uint32_t askQuantity;
	int32_t askPrice;
	Quote() = default;
	Quote(MessageHeader header , std::string symbol, uint64_t timestamp, uint32_t bidQuantity, uint32_t bidPrice, uint32_t askQuantity, int32_t askPrice);
	~Quote();

};