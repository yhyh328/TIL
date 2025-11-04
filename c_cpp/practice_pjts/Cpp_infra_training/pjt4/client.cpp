#include <iostream>
#include <string>
#include <random>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdexcept>

class SensorClient {
private:
    int sock;
    std::string host;
    int port;
    std::mt19937 gen;
    std::uniform_real_distribution<float> temp_dist;
    std::uniform_real_distribution<float> volt_dist;
    
public:
    SensorClient(const std::string& host = "127.0.0.1", int port = 8080)
        : sock(-1), host(host), port(port), 
          gen(std::random_device{}()),
          temp_dist(20.0f, 30.0f),
          volt_dist(100.0f, 150.0f) {}
    
    ~SensorClient() {
        if (sock >= 0) {
            close(sock);
        }
    }
    
    void connect() {
        sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0) {
            throw std::runtime_error("socket creation failed");
        }
        
        struct sockaddr_in serv = {0};
        serv.sin_family = AF_INET;
        serv.sin_port = htons(port);
        if (inet_pton(AF_INET, host.c_str(), &serv.sin_addr) <= 0) {
            close(sock);
            throw std::runtime_error("inet_pton failed");
        }
        
        if (::connect(sock, (struct sockaddr*)&serv, sizeof(serv)) < 0) {
            close(sock);
            throw std::runtime_error("connect failed");
        }
    }
    
    void sendSensorData() {
        float temp = temp_dist(gen);
        float volt = volt_dist(gen);
        
        std::string data = "TEMP=" + std::to_string(temp) + 
                          ",VOLT=" + std::to_string(volt) + "\n";
        
        if (write(sock, data.c_str(), data.length()) < 0) {
            throw std::runtime_error("write failed");
        }
    }
    
    void run() {
        std::cout << "Connected to server. Sending sensor data..." << std::endl;
        while (true) {
            sendSensorData();
            sleep(3);
        }
    }
};

SensorClient* g_client = nullptr;

void cleanup(int sig) {
    if (g_client) {
        delete g_client;
    }
    exit(0);
}

int main() {
    signal(SIGINT, cleanup);
    
    try {
        g_client = new SensorClient("127.0.0.1", 8080);
        g_client->connect();
        g_client->run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        delete g_client;
        return 1;
    }
    
    delete g_client;
    return 0;
}