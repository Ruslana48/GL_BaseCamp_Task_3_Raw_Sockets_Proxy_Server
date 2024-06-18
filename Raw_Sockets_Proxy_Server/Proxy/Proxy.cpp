#include "Proxy/Proxy.h"

Proxy::Proxy(const std::string& proxy_ip, int proxy_port, const std::string& server_ip, int server_port) {
    rawSocket.initializeSocket(sockfd, proxy_addr, proxy_ip, proxy_port); // sockfd overwritten
    rawSocket.initializeSocket(sockfd, server_addr, server_ip, server_port);
}

void Proxy::receivePacket() {
    struct sockaddr_in sender_addr{};
    socklen_t len = sizeof(sender_addr);
    packetSize = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&sender_addr, &len);
    if (packetSize < 0) {
        perror("Proxy: failed to receive packet");
    } else {
        std::cout << "Proxy: packets received\n";
    }
}

bool Proxy::chooseModification(bool modify) {
    if (modify) {
        return modifyPayload();
    } else {
        return forwardPacket();
    }
}

bool Proxy::forwardPacket() { // reuse in modifyPayload
    struct iphdr* ip_header = (struct iphdr*)buffer;
    struct tcphdr* tcp_header = (struct tcphdr*)(buffer + ip_header->ihl * 4);
    // change destination
    if (tcp_header->dest != proxy_addr.sin_port) {
        // skip
        return false;
    }

    tcp_header->dest = server_addr.sin_port;
    std::string payload(reinterpret_cast<char*>(buffer + sizeof(iphdr)), packetSize - sizeof(iphdr)); // not needed

    if (sendto(sockfd, buffer + sizeof(iphdr), payload.size(), 0, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr<<"Proxy: failed to forward packet\n";
        return false;
    } else {
        std::cout << "Proxy: forwarded packet to server\n";
    }

    return true;
}


bool Proxy::modifyPayload() {
    struct iphdr* ip_header = (struct iphdr*)buffer;
    struct tcphdr* tcp_header = (struct tcphdr*)(buffer + ip_header->ihl * 4);

    if (tcp_header->dest != proxy_addr.sin_port) {
        // skip
        return false;
    }

    tcp_header->dest = server_addr.sin_port;

    std::string payload(reinterpret_cast<char*>(buffer + sizeof(iphdr)), packetSize - sizeof(iphdr));
    payload += " changed";
    
    if (sendto(sockfd, payload.c_str(), payload.size(), 0, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr<<"Proxy: failed to resend modified packet to server";
    } else {
        std::cout << "Proxy: modified packet sent to server\n";
    }
    return true;
}

void Proxy::run(bool modify) {
    while (true) {
        //RawSocket::receivePacket(sockfd, );
        receivePacket();
        if (packetSize > 0) {
            if (chooseModification(modify)) break;
        }
    }
}
