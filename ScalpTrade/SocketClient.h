#pragma once
#include <string>
#include <cstdint>
#include "ConcurrentQueue.h"

template <typename T>
class SocketClient
{
private:
	std::string tcpIp;
	std::string tcpPort;
	ConcurrentQueue<char*> queue;
	int status;
	int client_fd;
	int tcpConnectionAlive;
	char buffer[1024];
public:
	SocketClient(std::string tcpIp , std::string tcpPort, ConcurrentQueue<char*> queue);
	~SocketClient();
	int startReceivingData();
	int startSendingData();
};