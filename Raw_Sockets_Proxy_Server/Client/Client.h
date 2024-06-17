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

class Client {
    private:
        int sockfd{};
        std::string message;
        struct sockaddr_in proxy_addr, client_addr;
        char buffer[4096];
        struct iphdr* ip_header;
        struct tcphdr* tcp_header;
        RawSocket rawSocket;

        int proxy_port;
        int messageSize;

        void initialize_TCP_header();
        void send_to_proxy();

        tcphdr * tcpHeader() {return reinterpret_cast<tcphdr *>(buffer);}
        char * data() {return reinterpret_cast<char *>(tcpHeader()) + sizeof(tcphdr);}
        int maxPayload() {return sizeof(buffer) - sizeof(tcphdr);}
        int packetSize() {return messageSize + sizeof(tcphdr);}

    public:
        Client(const std::string& client_ip, int client_port, const std::string& proxy_ip, int proxy_port);
        
        void run(std::string message);
};
