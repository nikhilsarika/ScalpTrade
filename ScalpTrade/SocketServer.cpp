#include "SocketServer.h"

#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "TestVariables.h"
#include "Trade.h"



SocketServer::SocketServer(std::string tcpIp, std::string tcpPort) : tcpIp(tcpIp), tcpPort(tcpPort)
{
    tcpConnectionAlive = 1;
    opt =1;
};


SocketServer::~SocketServer()
{
    // closing the connected socket
    close(new_socket);
    // closing the listening socket
    shutdown(server_fd, SHUT_RDWR);
};


int SocketServer::sendMessages() {

    std::cout << "Begining of Send Messages: " << '\n';
    struct sockaddr_in address;
    addrlen = sizeof(address);

    // Creating socket file descriptor
    std::cout << "before Socket Filse Desrciptor Creation: " << '\n';
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    std::cout << "before Forefullyt attaching port: " << '\n';
    if (setsockopt(server_fd, SOL_SOCKET,
        SO_REUSEADDR , &opt,
        sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(std::stoul(tcpPort));


    std::cout << "before Binding: " << '\n';
    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr*)&address,
        sizeof(address))
        < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    std::cout << "before Listen: " << '\n';
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    std::cout << "before accept: " << '\n';
    if ((new_socket
        = accept(server_fd, (struct sockaddr*)&address,
            (socklen_t*)&addrlen))
        < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    std::cout << "before ifinite while: " << '\n';
    while (tcpConnectionAlive) {
        if(!testQueue->isEmpty()){
            tcpConnectionAlive = send(new_socket, testQueue->pop(), 1024, 0);
        }
    }  
    return 0;

};

