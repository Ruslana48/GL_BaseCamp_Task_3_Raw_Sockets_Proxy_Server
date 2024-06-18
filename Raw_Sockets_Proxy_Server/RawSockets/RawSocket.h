#pragma once
#include <iostream>
#include <string>
#include <arpa/inet.h>
#include <unistd.h>

// make it a real class with its own values.
class RawSocket {
    public:
        void initializeSocket(int& sockfd, struct sockaddr_in& addr, const std::string& ip, int port);
        int sendPacket(int sockfd, const char* packet, int packetSize, struct sockaddr_in* dest_addr);
        int receivePacket(int sockfd, char* buffer, int bufferSize, struct sockaddr_in* src_addr);
        void closeSocket(int sockfd);
        static unsigned short calculateChecksum(unsigned short* buffer, int size);
};