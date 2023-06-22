#pragma once
#include <cstdint>
class MessageHeader
{
private:


public:
	uint8_t length;
	uint8_t type;
	MessageHeader(uint8_t length, uint8_t type);
	~MessageHeader();
};

