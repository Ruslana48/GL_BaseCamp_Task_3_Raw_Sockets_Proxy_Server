#pragma once
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include "../RawSockets/RawSocket.h"

#define BUFFER_SIZE 4096

class Server {
    private:
        int sockfd;
        struct sockaddr_in serv_addr;
        char buffer[4096];
        int packetSize;
        RawSocket rawSocket;

        void receivePacket();
        bool processPacket();
        unsigned short calculateChecksum(unsigned short* buffer, int size);
        
    public:
        Server(const std::string& server_ip, int server_port);
        void run(bool modify);
};

