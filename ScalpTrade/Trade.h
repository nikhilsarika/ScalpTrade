#pragma once
#include <string>
#include <cstdint>
#include "MessageHeader.h"
class Trade 
{
private:

public:
	MessageHeader* header;
	std::string symbol;
	uint64_t timestamp;
	uint32_t quantity;
	int32_t price;
	Trade(MessageHeader* header, std::string symbol, uint64_t timestamp, uint32_t quantity, int32_t price);
	~Trade();
};