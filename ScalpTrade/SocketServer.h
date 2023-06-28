#pragma once
#include <string>
#include <cstdint>

class SocketServer
{
private:
	std::string tcpIp;
	std::string tcpPort;
	int server_fd;
	int new_socket;
	int valread;
	int tcpConnectionAlive;
	int opt;
	int addrlen;
public:
	SocketServer(std::string tcpIp, std::string tcpPort);
	~SocketServer();
	int sendMessages();
	int receiveMessages();
};