# 📘 C++ 실무 감각 훈련 프로젝트 가이드  
**작성자:** 김요한  
**목적:** 큐슈전력/TST 인프라 파견을 대비한 실무형 C++ 환경 및 구조 익히기  
**언어:** C++ (C++11 이상)  
**환경:** AlmaLinux / WSL / g++ / Makefile 기반

---

## 🧩 1단계 — 리눅스 시스템 감시 도구 만들기 (Mini `top`)

### 🎯 목표  
- `/proc` 파일시스템에서 CPU, 메모리, 프로세스 정보를 읽어  
  간단한 시스템 모니터링 CLI 도구를 만든다.

### 🔧 학습 포인트  
- C++ 파일 입출력 (`fstream`, `ifstream`)  
- 문자열 처리 (`std::string`, `std::stringstream`)  
- 클래스와 캡슐화  
- STL 컨테이너 활용

### 📂 구조  
```
sysmon/
 ├── main.cpp
 ├── sysinfo.cpp
 ├── sysinfo.h
 └── Makefile
```

### 📄 코드 예시

**main.cpp**
```cpp
#include <iostream>
#include <stdexcept>
#include "sysinfo.h"

int main() {
    try {
        SysInfo info;
        info.getCpuMemInfo();
        info.printSysInfo();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
```

**sysinfo.h**
```cpp
#ifndef SYSINFO_H
#define SYSINFO_H

class SysInfo {
private:
    double cpu_usage;
    double mem_usage;
    
    void parseCpuInfo();
    void parseMemInfo();

public:
    SysInfo() : cpu_usage(0.0), mem_usage(0.0) {}
    
    void getCpuMemInfo();
    void printSysInfo() const;
    
    double getCpuUsage() const { return cpu_usage; }
    double getMemUsage() const { return mem_usage; }
};

#endif
```

**sysinfo.cpp**
```cpp
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <stdexcept>
#include "sysinfo.h"

void SysInfo::parseCpuInfo() {
    std::ifstream fp("/proc/stat");
    if (!fp.is_open()) {
        throw std::runtime_error("Cannot open /proc/stat");
    }
    
    unsigned long long user, nice, system, idle;
    std::string cpu_label;
    fp >> cpu_label >> user >> nice >> system >> idle;
    fp.close();
    
    cpu_usage = 100.0 * (user + nice + system) / (user + nice + system + idle);
}

void SysInfo::parseMemInfo() {
    std::ifstream fp("/proc/meminfo");
    if (!fp.is_open()) {
        throw std::runtime_error("Cannot open /proc/meminfo");
    }
    
    std::string label;
    unsigned long total, free;
    
    fp >> label >> total >> label;
    fp >> label >> free >> label;
    fp.close();
    
    mem_usage = 100.0 * (total - free) / total;
}

void SysInfo::getCpuMemInfo() {
    parseCpuInfo();
    parseMemInfo();
}

void SysInfo::printSysInfo() const {
    std::cout << "CPU Usage: " << std::fixed << std::setprecision(2) 
              << cpu_usage << "%" << std::endl;
    std::cout << "Memory Usage: " << std::fixed << std::setprecision(2) 
              << mem_usage << "%" << std::endl;
}
```

**Makefile**
```make
CXX=g++
CXXFLAGS=-Wall -O2 -std=c++11

sysmon: main.o sysinfo.o
	$(CXX) $(CXXFLAGS) -o sysmon main.o sysinfo.o

clean:
	rm -rf *.o sysmon
```

---

## 🌐 2단계 — TCP/IP 서버-클라이언트

### 🎯 목표  
로컬 네트워크에서 서버와 클라이언트가 텍스트를 주고받는 프로그램 제작

### 🔧 학습 포인트  
- 소켓 클래스 설계  
- 예외 처리 (`try-catch`, `std::runtime_error`)  
- `std::string` 활용  
- RAII 패턴 (Resource Acquisition Is Initialization)

### 📄 서버 코드 예시 (`server.cpp`)
```cpp
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
        server_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (server_fd < 0) {
            throw std::runtime_error("socket creation failed");
        }
        
        struct sockaddr_in addr = {0};
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = INADDR_ANY;
        
        if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
            close(server_fd);
            throw std::runtime_error("bind failed");
        }
        
        if (listen(server_fd, 5) < 0) {
            close(server_fd);
            throw std::runtime_error("listen failed");
        }
        
        std::cout << "Server listening on port " << port << "..." << std::endl;
    }
    
    void run() {
        while (true) {
            int client = accept(server_fd, nullptr, nullptr);
            if (client < 0) {
                std::cerr << "accept failed" << std::endl;
                continue;
            }
            
            char buf[256] = {0};
            int n = read(client, buf, sizeof(buf) - 1);
            if (n > 0) {
                std::cout << "Received: " << buf;
                write(client, "OK\n", 3);
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
```

### 📄 클라이언트 예시 (`client.cpp`)
```cpp
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
```

---

## 🧰 3단계 — 로그 관리 & 설정 파일 시스템

### 🎯 목표  
- 설정파일(`config.conf`)을 읽어 동작 파라미터를 적용하고,  
  프로그램 로그를 날짜별로 저장한다.

### 🔧 학습 포인트  
- `std::map`을 활용한 설정 관리  
- 파일 시스템 라이브러리 (`<filesystem>` 또는 `<sys/stat.h>`)  
- `std::string` 파싱  
- RAII를 활용한 파일 처리

### 📄 config.conf 예시
```
LOG_PATH=./logs
INTERVAL=5
```

### 📄 코드 요약 (`main.cpp`)
```cpp
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdexcept>

class ConfigManager {
private:
    std::map<std::string, std::string> config;
    
public:
    void loadConfig(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Cannot open config.conf");
        }
        
        std::string line;
        while (std::getline(file, line)) {
            size_t pos = line.find('=');
            if (pos != std::string::npos) {
                std::string key = line.substr(0, pos);
                std::string value = line.substr(pos + 1);
                config[key] = value;
            }
        }
        file.close();
    }
    
    std::string get(const std::string& key, const std::string& default_value = "") const {
        auto it = config.find(key);
        return (it != config.end()) ? it->second : default_value;
    }
    
    int getInt(const std::string& key, int default_value = 0) const {
        auto it = config.find(key);
        return (it != config.end()) ? std::stoi(it->second) : default_value;
    }
};

class Logger {
private:
    std::string log_path;
    
    void ensureDirectoryExists(const std::string& path) {
        struct stat st = {0};
        if (stat(path.c_str(), &st) == -1) {
            mkdir(path.c_str(), 0755);
        }
    }
    
    std::string getLogFilename() {
        std::time_t t = std::time(nullptr);
        std::tm* tm = std::localtime(&t);
        
        std::ostringstream oss;
        oss << log_path << "/log_" 
            << std::setfill('0') << std::setw(4) << (tm->tm_year + 1900)
            << std::setw(2) << (tm->tm_mon + 1)
            << std::setw(2) << tm->tm_mday << ".txt";
        return oss.str();
    }
    
public:
    Logger(const std::string& path) : log_path(path) {
        ensureDirectoryExists(log_path);
    }
    
    void writeLog(const std::string& msg) {
        std::string filename = getLogFilename();
        std::ofstream file(filename, std::ios::app);
        if (!file.is_open()) {
            std::cerr << "Error: Cannot open log file " << filename << std::endl;
            return;
        }
        
        std::time_t t = std::time(nullptr);
        std::tm* tm = std::localtime(&t);
        
        file << std::setfill('0') << std::setw(2) << tm->tm_hour << ":"
             << std::setw(2) << tm->tm_min << ":"
             << std::setw(2) << tm->tm_sec << " " << msg << std::endl;
        file.close();
        
        std::cout << std::setfill('0') << std::setw(2) << tm->tm_hour << ":"
                  << std::setw(2) << tm->tm_min << ":"
                  << std::setw(2) << tm->tm_sec << " Log written: " << msg << std::endl;
    }
};

int main() {
    try {
        ConfigManager config;
        config.loadConfig("config.conf");
        
        std::string log_path = config.get("LOG_PATH", "./logs");
        int interval = config.getInt("INTERVAL", 5);
        
        Logger logger(log_path);
        
        std::cout << "Logging service started. Log path: " << log_path 
                  << ", Interval: " << interval << " seconds" << std::endl;
        std::cout << "Press Ctrl+C to stop." << std::endl << std::endl;
        
        while (true) {
            logger.writeLog("System OK");
            sleep(interval);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
```

---

## ⚡ 4단계 — 센서 시뮬레이터

### 🎯 목표  
- 가상의 센서 데이터를 생성하고 TCP 서버로 전송  
- 일정 주기마다 온도·전압·전류 등 난수 데이터를 송신

### 📄 코드 예시 (`sensor_sim.cpp`)

#### 포트 자동 할당 기능이 포함된 서버 코드

**주요 개념:**
- C++ 클래스로 소켓 추상화
- 예외 처리로 에러 관리
- RAII로 자원 관리
- 포트 8080이 점유 중이면 자동으로 다음 포트 검색 (8080~65535)

```cpp
#include <iostream>
#include <string>
#include <random>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdexcept>

class SensorServer {
private:
    int sock;
    int port;
    
public:
    SensorServer() : sock(-1), port(0) {}
    
    ~SensorServer() {
        if (sock >= 0) {
            close(sock);
        }
    }
    
    int findAvailablePort(int start_port = 8080) {
        sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0) {
            throw std::runtime_error("socket creation failed");
        }
        
        int opt = 1;
        if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
            close(sock);
            throw std::runtime_error("setsockopt failed");
        }
        
        struct sockaddr_in serv = {0};
        serv.sin_family = AF_INET;
        serv.sin_addr.s_addr = INADDR_ANY;
        
        for (port = start_port; port <= 65535; port++) {
            serv.sin_port = htons(port);
            errno = 0;
            if (bind(sock, (struct sockaddr*)&serv, sizeof(serv)) == 0) {
                std::cout << "Successfully bound to port " << port << std::endl;
                return port;
            }
            if (errno != EADDRINUSE) {
                close(sock);
                throw std::runtime_error("bind failed");
            }
        }
        
        close(sock);
        throw std::runtime_error("No available port found (8080-65535)");
    }
    
    void listen() {
        if (::listen(sock, 5) < 0) {
            close(sock);
            throw std::runtime_error("listen failed");
        }
        std::cout << "Server listening on port " << port << "..." << std::endl;
    }
    
    void run() {
        while (true) {
            int client = accept(sock, nullptr, nullptr);
            if (client < 0) {
                std::cerr << "accept failed" << std::endl;
                continue;
            }
            
            char buf[256] = {0};
            int n = read(client, buf, sizeof(buf) - 1);
            if (n > 0) {
                std::cout << "Received sensor data: " << buf;
                write(client, "ACK\n", 4);
            }
            close(client);
        }
    }
};

int main() {
    try {
        SensorServer server;
        server.findAvailablePort();
        server.listen();
        server.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
```

#### 클라이언트 연결 예시 (`client.cpp`)

**개선 사항:**
- C++ 클래스로 소켓 추상화
- 시그널 핸들러로 안전한 종료
- `std::string` 활용
- 예외 처리로 에러 관리

```cpp
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
```

---

## 🧱 마무리
이 1~4단계는 **실제 산업용 코드 구조**를 단순화한 버전입니다.  
직접 타이핑하며 컴파일하고, 각 단계에서  
- 클래스 설계와 캡슐화
- 예외 처리와 RAII 패턴
- STL 활용 (`std::string`, `std::map`, `std::fstream`)
- Makefile 수정 (g++ 사용)
- 로그·출력·네트워크 동작 확인  
을 경험하면, 겐바 투입 전에 충분히 실전 감각을 익힐 수 있습니다.

**C++ 특징 요약:**
- **클래스와 캡슐화**: 데이터와 함수를 하나의 단위로 묶어 관리
- **RAII**: 생성자/소멸자로 자원 자동 관리
- **예외 처리**: `try-catch`로 에러 처리
- **STL**: 표준 라이브러리 활용으로 생산성 향상
- **네임스페이스**: 코드 모듈화 및 충돌 방지

