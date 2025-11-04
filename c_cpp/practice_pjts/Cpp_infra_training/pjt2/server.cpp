#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdexcept>

class TcpServer {
private:
    int server_fd;
    int port;

public:
    TcpServer(int port = 8081) : server_fd(-1), port(port) {}

    ~TcpServer() {
        if (server_fd >= 0) {
            close(server_fd);
        }
    }

    void bindAndListen() {
        // 1. Create socket
        server_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (server_fd < 0) {
            throw std::runtime_error("socket creation failed");
        }

        // 2. Set SO_REUSEADDR option to avoid "Address already in use" on restart
        int opt = 1;
        if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
            close(server_fd);
            throw std::runtime_error("setsockopt failed");
        }

        // 3. Prepare address structure
        struct sockaddr_in addr{};
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = INADDR_ANY;

        // 4. Bind socket to port
        if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
            close(server_fd);
            throw std::runtime_error("bind failed");
        }

        // 5. Start listening
        if (listen(server_fd, 5) < 0) {
            close(server_fd);
            throw std::runtime_error("listen failed");
        }

        std::cout << "✅ Server listening on port " << port << "..." << std::endl;
    }

    void run() {
        while (true) {
            int client = accept(server_fd, nullptr, nullptr);
            if (client < 0) {
                std::cerr << "❌ accept failed" << std::endl;
                continue;
            }

            char buf[256];
            ssize_t n = read(client, buf, sizeof(buf));
            if (n > 0) {
                std::string msg(buf, n);
                std::cout << "📩 Received: " << msg << std::flush;

                ssize_t sent = write(client, "OK\n", 3);
                if (sent < 0) perror("write");
            }

            close(client);
        }
    }
};

int main() {
    try {
        TcpServer server(8081);
        server.bindAndListen();
        server.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
