#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdexcept>

class TcpClient {
private:
    int sock;
    std::string host;
    int port;
    
public:
    TcpClient(const std::string& host = "127.0.0.1", int port = 8081) 
        : sock(-1), host(host), port(port) {}
    
    ~TcpClient() {
        if (sock >= 0) {
            close(sock);
        }
    }
    
    void connect() {
        sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0) {
            throw std::runtime_error("socket creation failed");
        }
        
        struct sockaddr_in server = {0};
        server.sin_family = AF_INET;
        server.sin_port = htons(port);
        if (inet_pton(AF_INET, host.c_str(), &server.sin_addr) <= 0) {
            close(sock);
            throw std::runtime_error("inet_pton failed");
        }
        
        if (::connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
            close(sock);
            throw std::runtime_error("connect failed");
        }
    }
    
    void sendMessage(const std::string& msg) {
        if (write(sock, msg.c_str(), msg.length()) < 0) {
            throw std::runtime_error("write failed");
        }
    }
    
    std::string receiveResponse() {
        char response[256];
        int n = read(sock, response, sizeof(response) - 1);
        if (n > 0) {
            response[n] = '\0';
            return std::string(response);
        }
        return "";
    }
};

int main() {
    try {
        TcpClient client("127.0.0.1", 8081);
        client.connect();
        
        std::string msg;
        std::cout << "Enter message: ";
        std::getline(std::cin, msg);
        
        client.sendMessage(msg);
        std::string response = client.receiveResponse();
        std::cout << "Server response: " << response;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}