#include <iostream>
#include <string>
#include <cstring>      // std::memset, strerror
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cerrno>
#include <stdexcept>

class SensorServer {
private:
    int sock;
    int port;

public:
    SensorServer() : sock(-1), port(0) {}

    ~SensorServer() {
        if (sock >= 0) close(sock);
    }

    int findAvailablePort(int start_port = 8080) {
        // 1) 소켓 생성
        sock = ::socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0) throw std::runtime_error("socket() failed");

        // 2) 재사용 옵션
        int opt = 1;
        if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
            int err = errno;
            close(sock);
            throw std::runtime_error(std::string("setsockopt(SO_REUSEADDR) failed: ") + std::strerror(err));
        }
        // 필요하면 SO_REUSEPORT도:
        // setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));

        // 3) 주소 구조체 값 초기화
        sockaddr_in serv{};
        serv.sin_family = AF_INET;
        serv.sin_addr.s_addr = htonl(INADDR_ANY);

        // 4) 포트 스캔
        for (port = start_port; port <= 65535; ++port) {
            serv.sin_port = htons(port);
            if (::bind(sock, reinterpret_cast<sockaddr*>(&serv), sizeof(serv)) == 0) {
                std::cout << "Successfully bound to port " << port << std::endl;
                return port;
            }
            if (errno != EADDRINUSE) {
                int err = errno;
                close(sock);
                throw std::runtime_error(std::string("bind() failed: ") + std::strerror(err));
            }
            // 포트 사용 중이면 다음 포트 시도
        }

        close(sock);
        throw std::runtime_error("No available port found in range 8080-65535");
    }

    void listen() {
        if (::listen(sock, 5) < 0) {
            int err = errno;
            close(sock);
            throw std::runtime_error(std::string("listen() failed: ") + std::strerror(err));
        }
        std::cout << "Server listening on port " << port << "..." << std::endl;
    }

    void run() {
        while (true) {
            int client = ::accept(sock, nullptr, nullptr);
            if (client < 0) {
                std::cerr << "accept() failed: " << std::strerror(errno) << std::endl;
                continue;
            }

            char buf[256];
            ssize_t n = ::read(client, buf, sizeof(buf));
            if (n > 0) {
                std::string msg(buf, static_cast<size_t>(n));
                std::cout << "Received sensor data: " << msg << std::flush;

                ssize_t sent = ::write(client, "ACK\n", 4);
                if (sent < 0) {
                    std::cerr << "write() failed: " << std::strerror(errno) << std::endl;
                }
            } else if (n < 0) {
                std::cerr << "read() failed: " << std::strerror(errno) << std::endl;
            }

            ::close(client);
        }
    }
};

int main() {
    try {
        SensorServer server;
        server.findAvailablePort();   // 기본 8080부터 스캔
        server.listen();
        server.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
