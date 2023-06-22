#pragma once
#include <string>
#include <cstdint>
class Order 
{
private:

public:
	std::string symbol;
	uint64_t timestamp;
	char side;
	uint32_t quantity;
	int32_t price;
	Order( std::string symbol, uint64_t timestamp, char side, uint32_t quantity, int32_t price);
	~Order();
};