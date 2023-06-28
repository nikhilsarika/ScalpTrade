#pragma once
#include <string>
#include "Quote.h"

class SocketClient
{
private:
	std::string tcpIp;
	std::string tcpPort;
	int status;
	int client_fd;
	int tcpConnectionAlive;
	char buffer[1024] = {0};
public:
	SocketClient(std::string tcpIp , std::string tcpPort);
	~SocketClient();
	int startReceivingData();
	int startSendingData();
};