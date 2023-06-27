#include "SocketClient.h"

#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

template <typename T>
SocketClient<T>::SocketClient(std::string tcpIp, std::string tcpPort, ConcurrentQueue<char*> queue) : tcpIp(tcpIp), tcpPort(tcpPort), queue(queue) 
{
    tcpConnectionAlive = 1;
    buffer = { 0 };
};

template <typename T>
SocketClient<T>::~SocketClient() 
{
    close(client_fd);
};

template <typename T>
int SocketClient<T>::startReceivingData() {
    struct sockaddr_in serv_addr;

    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(tcpPort);

    // Convert IPv4 and IPv6 addresses from text to binary
    // form
    if (inet_pton(AF_INET, tcpIp, &serv_addr.sin_addr)
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

    while (tcpConnectionAlive) {
        tcpConnectionAlive = read(client_fd, buffer, 1024);
        queue.push(buffer);
    }

    return 0;

}

template <typename T>
int SocketClient<T>::startSendingData() {
    struct sockaddr_in serv_addr;
   
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(tcpPort);

    // Convert IPv4 and IPv6 addresses from text to binary
    // form
    if (inet_pton(AF_INET, tcpIp, &serv_addr.sin_addr)
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
        if (!queue.isEmpty()) {
            buffer = queue.pop();
            tcpConnectionAlive = (client_fd, buffer, 1024, 0);
        }
    }
    
    return 0;

}