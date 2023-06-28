#pragma once
#include <string>
#include <cstdint>
class NewTrade 
{
private:

public:
	std::string symbol;
	uint64_t timestamp;
	uint32_t quantity;
	int32_t price;
	NewTrade() = default;
	NewTrade(std::string symbol, uint64_t timestamp, uint32_t quantity, int32_t price);
	~NewTrade();
};