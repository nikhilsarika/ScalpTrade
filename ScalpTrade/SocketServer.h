#pragma once
#include <string>
#include <cstdint>
#include "ConcurrentQueue.h"

template <typename T>
class SocketServer
{
private:
	std::string tcpIp;
	std::string tcpPort;
	ConcurrentQueue<char*> queue;
	int server_fd;
	int new_socket;
	int valread;
	int tcpConnectionAlive;
	char buffer[1024];
public:
	SocketServer(std::string tcpIp, std::string tcpPort, ConcurrentQueue<char*> queue);
	~SocketServer();
	int sendMessages();
};
};