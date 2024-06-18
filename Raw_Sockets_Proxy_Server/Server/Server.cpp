#include "../Server/Server.h"

Server::Server(const std::string& server_ip, int server_port) {
    rawSocket.initializeSocket(sockfd, serv_addr, server_ip, server_port);
}

void Server::receivePacket() {
    struct iphdr* ip_header = (struct iphdr*)buffer;
    struct tcphdr* tcp_header = (struct tcphdr*)(buffer + ip_header->ihl * 4);
    struct sockaddr_in sender_addr{};
    socklen_t len = sizeof(sender_addr);
    packetSize = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&sender_addr, &len);
    if (packetSize < 0) {
        perror("Failed to receive packet");
    }
    else{
        std::cout<<"Server: received packet, Length: "<<packetSize<<std::endl;
    }
}
bool Server::processPacket() {
    struct iphdr* ip_header = (struct iphdr*)buffer;
    struct tcphdr* tcp_header = (struct tcphdr*)(buffer + ip_header->ihl * 4);
    char* data = buffer + ip_header->ihl * 4 + tcp_header->doff * 4;

    if (tcp_header->dest != serv_addr.sin_port) {
        // skip
        return false;
    }
    
    unsigned short received_checksum = tcp_header->check;
    tcp_header->check = 0;
    unsigned short calculated_checksum = rawSocket.calculateChecksum((unsigned short*)buffer, packetSize / 2);
    
    tcp_header->check = received_checksum;
    std::cout << "Server received: " << data << std::endl;
    std::cout << "Checksum (received): " << received_checksum << std::endl;
    std::cout << "Checksum (calculated): " << calculated_checksum << std::endl;

    if (received_checksum == calculated_checksum) {
        std::cout << "Server: Checksum matches. Payload: " << data << std::endl;
    } else {
        std::cout << "Server: Checksum does not match. Payload was changed by proxy." << std::endl;
    }
    return true;
}

void Server::run(bool modify) {
    while (true) {
        receivePacket();
        if (modify) { // not needed
            if (processPacket()) break;
        }
    }
}