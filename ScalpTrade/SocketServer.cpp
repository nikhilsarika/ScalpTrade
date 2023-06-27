#include "SocketServer.h"

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>


template <typename T>
SocketServer<T>::SocketServer(std::string tcpIp, std::string tcpPort, ConcurrentQueue<char*> queue) : tcpIp(tcpIp), tcpPort(tcpPort), queue(queue)
{
    tcpConnectionAlive = 1;
    buffer = { 0 };
};

template <typename T>
SocketServer<T>::~SocketServer()
{
    // closing the connected socket
    close(new_socket);
    // closing the listening socket
    shutdown(server_fd, SHUT_RDWR);
};

template <typename T>
int SocketServer<T>::sendMessages() {
    struct sockaddr_in address;

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET,
        SO_REUSEADDR | SO_REUSEPORT, &opt,
        sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr*)&address,
        sizeof(address))
        < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket
        = accept(server_fd, (struct sockaddr*)&address,
            (socklen_t*)&addrlen))
        < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }


    while (tcpConnectionAlive < 0) {
        tcpConnectionAlive = send(new_socket, buffer, 1024, 0);
    }

    return 0;

}

