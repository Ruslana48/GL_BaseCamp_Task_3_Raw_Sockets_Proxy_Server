#pragma once

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/tcp.h>
#include <netinet/ip.h>
#include "../RawSockets/RawSocket.h"

#define BUFFER_SIZE 4096

class Proxy {
    private:
        bool modify;
        int sockfd;
        struct sockaddr_in proxy_addr, server_addr;
        char buffer[4096];
        int packetSize;
        RawSocket rawSocket;
        struct iphdr *ip_header{};
        struct tcphdr *tcp_header{};

        void initializeProxySocket(const std::string& server_ip, int server_port);
        void initializeServerSocket(const std::string& proxy_ip, int proxy_port);
        bool chooseModification(bool modify);
        bool processPacket(bool modify);
        void receivePacket();
        bool forwardPacket();
        bool modifyPayload();

        unsigned short calculateChecksum(unsigned short* buffer, int size);
    
        
    public:
        Proxy(const std::string& proxy_ip, int proxy_port, const std::string& server_ip, int server_port);
        void run(bool modify);
};