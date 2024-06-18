#include "Client.h"
#include "../RawSockets/RawSocket.h"

Client::Client(const std::string& client_ip, int client_port, const std::string& proxy_ip, int proxy_port) {
    rawSocket.initializeSocket(sockfd, client_addr, client_ip, client_port);
    rawSocket.initializeSocket(sockfd, proxy_addr, proxy_ip, proxy_port);

    ip_header = reinterpret_cast<struct iphdr*>(buffer);
    tcp_header = reinterpret_cast<struct tcphdr*>(buffer + sizeof(struct iphdr));
}

void Client::initialize_TCP_header() {
    tcp_header = tcpHeader();
    tcp_header->source = htons(12345);
    tcp_header->dest = proxy_addr.sin_port;
    tcp_header->seq = 0;
    tcp_header->ack_seq = 0;
    tcp_header->doff = 5;
    tcp_header->fin = 0;
    tcp_header->syn = 1;
    tcp_header->rst = 0;
    tcp_header->psh = 0;
    tcp_header->ack = 0;
    tcp_header->urg = 0;
    tcp_header->window = htons(5840);
    tcp_header->check = 0;
    tcp_header->urg_ptr = 0;
}
// Buffer
// TCPHeader|Payload

// PseudoHeader|TcpHeader|Payload

// PseudoHeader - erased IPHeader
void Client::run(std::string message) {
    initialize_TCP_header();

    strncpy(data(), message.c_str(), maxPayload());
    messageSize = message.size();

    tcpHeader()->check=RawSocket::calculateChecksum(reinterpret_cast<unsigned short*>(buffer), packetSize()/2);

    rawSocket.sendPacket(sockfd, buffer, packetSize(), (struct sockaddr_in*)&proxy_addr);
}
