#include "SocketClient.h"

#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "global.h"


SocketClient::SocketClient(std::string tcpIp, std::string tcpPort) : tcpIp(tcpIp), tcpPort(tcpPort)
{
    tcpConnectionAlive = 1;
};


SocketClient::~SocketClient() 
{
    close(client_fd);
};


int SocketClient::startReceivingData() {
    struct sockaddr_in serv_addr;

    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(std::stoul(tcpPort));
    serv_addr.sin_addr.s_addr= inet_addr(tcpIp.c_str());
    // Convert IPv4 and IPv6 addresses from text to binary
    // form
    // if (inet_pton(AF_INET, tcpIp.c_str(), &serv_addr.sin_addr)
    //     <= 0) {
    //     printf(
    //         "\nInvalid address/ Address not supported \n");
    //     return -1;
    // }
    std::cout << "Value of Cleint Fd: " << client_fd << '\n';
    if ((status = connect(client_fd, (struct sockaddr*)&serv_addr,sizeof(serv_addr)))< 0) {
        std::cout << "connect value after attemptint to connect " << status << '\n';
        printf("\nConnection Failed \n");
        return -1;
    }
    std::cout << "Before while in client " << '\n';
    while (tcpConnectionAlive) {
        char *charPointer;
        tcpConnectionAlive = read(client_fd, charPointer, 1024);

        inputByteQueue->push(charPointer);
    }

    return 0;

}


int SocketClient::startSendingData() {
    struct sockaddr_in serv_addr;
   
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(std::stoul(tcpPort));

    // Convert IPv4 and IPv6 addresses from text to binary
    // form
    if (inet_pton(AF_INET, tcpIp.c_str(), &serv_addr.sin_addr)
        <= 0) {
        printf(
            "\nInvalid address/ Address not supported \n");
        return -1;
    }

    if ((status
        = connect(client_fd, (struct sockaddr*)&serv_addr,
            sizeof(serv_addr)))
        < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }
    while (tcpConnectionAlive < 0) {
        if (!outputByteQueue->isEmpty()) {
            strcpy(buffer, outputByteQueue->pop());
            tcpConnectionAlive = send(client_fd, buffer, 1024, 0);
        }
    }
    
    return 0;

}