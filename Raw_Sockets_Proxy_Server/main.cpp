#include "../Server/Server.h"
#include "../Client/Client.h"
#include "../Proxy/Proxy.h"
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <iostream>

const int SERVER_PORT = 8080;
const int PROXY_PORT = 8081;
const char* IP = "127.0.0.1";
const int CLIENT_PORT = 8082;

std::mutex mtx;
std::condition_variable cv;
std::atomic<bool> client_done(false);
std::atomic<bool> proxy_done(false);

void menu() {
    std::cout << "Choose option: " << std::endl;
    std::cout << "1. Proxy just forwards data to the receiver" << std::endl;
    std::cout << "2. Proxy does modifications in payload" << std::endl;
    std::cout << "Type your choice here: ";
}

void runClient(Client& client, const std::string& message) {
    client.run(message);
    {
        std::lock_guard<std::mutex> lock(mtx);
        client_done = true;
    }
    cv.notify_all();
}

void runProxy(Proxy& proxy, bool modify) {
    {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [] { return client_done.load(); });
    }
    proxy.run(modify);
    {
        std::lock_guard<std::mutex> lock(mtx);
        proxy_done = true;
    }
    cv.notify_all();
}

void runServer(Server& server, bool modify) {
    {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [] { return proxy_done.load(); });
    }
    server.run(modify);
}

int main() {
    std::string choice;
    std::string message;
    menu();
    std::cin >> choice;
    
    int option = std::stoi(choice);
    if(option != 1 && option != 2){
        std::cout << "You input the wrong number\n";
        return 0;
    }

    bool modify = (option == 2);

    std::cout << "Enter the payload data: ";
    std::cin >> message;

    Client client(IP, CLIENT_PORT, IP, PROXY_PORT);
    Proxy proxy(IP, PROXY_PORT, IP, SERVER_PORT);
    Server server(IP, SERVER_PORT); 

    std::thread clientThread(runClient, std::ref(client), std::ref(message));
    std::thread proxyThread(runProxy, std::ref(proxy), modify);
    std::thread serverThread(runServer, std::ref(server), modify);

    if (serverThread.joinable()) serverThread.join();
    if (proxyThread.joinable()) proxyThread.join();
    if (clientThread.joinable()) clientThread.join();

    return 0;
}
