#include "RawSocket.h"

void RawSocket::initializeSocket(int& sockfd, struct sockaddr_in& addr, const std::string& ip, int port) {
    sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
    if (sockfd < 0) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    if (inet_pton(AF_INET, ip.c_str(), &addr.sin_addr) <= 0) {
        perror("Invalid address");
        exit(EXIT_FAILURE);
    }
}

int RawSocket::sendPacket(int sockfd, const char* packet, int packetSize, struct sockaddr_in* dest_addr) {
    return sendto(sockfd, packet, packetSize, 0, (struct sockaddr*)dest_addr, sizeof(*dest_addr));
}

int RawSocket::receivePacket(int sockfd, char* buffer, int bufferSize, struct sockaddr_in* sender_addr) {
    socklen_t len = sizeof(sender_addr);
    return recvfrom(sockfd, buffer, bufferSize, 0, (struct sockaddr*)&sender_addr, &len);
    
    //socklen_t addr_len = sizeof(*src_addr);
    //return recvfrom(sockfd, buffer, bufferSize, 0, (struct sockaddr*)src_addr, &addr_len);
}

void RawSocket::closeSocket(int sockfd) {
    close(sockfd);
}

unsigned short RawSocket::calculateChecksum(unsigned short* buffer, int size) {
    unsigned long sum = 0;
    for (int i = 0; i < size; i++) {
        sum += buffer[i];
        if (sum & 0xFFFF0000) {
            sum &= 0xFFFF;
            sum++;
        }
    }
    return ~(sum & 0xFFFF);
}