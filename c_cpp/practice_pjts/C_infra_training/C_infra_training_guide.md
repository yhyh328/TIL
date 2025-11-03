
# 📘 C 실무 감각 훈련 프로젝트 가이드  
**작성자:** 김요한  
**목적:** 큐슈전력/TST 인프라 파견을 대비한 실무형 C 환경 및 구조 익히기  
**언어:** C  
**환경:** AlmaLinux / WSL / gcc / Makefile 기반

---

## 🧩 1단계 — 리눅스 시스템 감시 도구 만들기 (Mini `top`)

### 🎯 목표  
- `/proc` 파일시스템에서 CPU, 메모리, 프로세스 정보를 읽어  
  간단한 시스템 모니터링 CLI 도구를 만든다.

### 🔧 학습 포인트  
- 파일 입출력 (`fopen`, `fgets`, `fscanf`)  
- 문자열 파싱  
- 구조체 사용  
- 반복문 / 조건문 활용  

### 📂 구조  
```
sysmon/
 ├── main.c
 ├── sysinfo.c
 ├── sysinfo.h
 └── Makefile
```

### 📄 코드 예시

**main.c**
```c
#include <stdio.h>
#include "sysinfo.h"

int main(void) {
    struct SysInfo info;
    get_cpu_mem_info(&info);
    print_sysinfo(&info);
    return 0;
}
```

**sysinfo.h**
```c
#ifndef SYSINFO_H
#define SYSINFO_H

struct SysInfo {
    double cpu_usage;
    double mem_usage;
};

void get_cpu_mem_info(struct SysInfo *info);
void print_sysinfo(const struct SysInfo *info);

#endif
```

**sysinfo.c**
```c
#include <stdio.h>
#include <stdlib.h>
#include "sysinfo.h"

void get_cpu_mem_info(struct SysInfo *info) {
    
    FILE *fp;
    
    unsigned long long user, nice, system, idle;
    
    fp = fopen("/proc/stat", "r");
    fscanf(fp, "cpu %llu %llu %llu %llu", &user, &nice, &system, &idle);
    fclose(fp);
    info->cpu_usage = 100.0 * (user + nice + system) / (user + nice + system + idle);

    fp = fopen("/proc/meminfo", "r");
    unsigned long total, free;
    fscanf(fp, "MemTotal: %lu kb", &total);
    fscanf(fp, "MemFree: %lu kb", &free);
    fclose(fp);
    info->mem_usage = 100.0 * (total - free) / total;

}

void print_sysinfo(const struct SysInfo *info) {
    printf("CPU Usage: %.2f%%\n", info->cpu_usage);
    printf("Memory Usage: %.2f%%\n", info->mem_usage);
}
```

**Makefile**
```make
CC=gcc
CFLAGS=-Wall -O2

sysmon: main.o sysinfo.o
	$(CC) $(CFLAGS) -o sysmon main.o sysinfo.o

clean:
	rm -rf *.o sysmon
```

---

## 🌐 2단계 — TCP/IP 서버-클라이언트

### 🎯 목표  
로컬 네트워크에서 서버와 클라이언트가 텍스트를 주고받는 프로그램 제작

### 🔧 학습 포인트  
- 소켓(`socket`, `bind`, `listen`, `accept`)  
- IP 주소 구조체(`sockaddr_in`)  
- 멀티클라이언트 처리 (`fork` 또는 `select`)

### 📄 서버 코드 예시 (`server.c`)
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket");
        return 1;
    }

    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8081);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind");
        close(server_fd);
        return 1;
    }

    if (listen(server_fd, 5) < 0) {
        perror("listen");
        close(server_fd);
        return 1;
    }

    printf("Server listening on port 8081...\n");
    while (1) {
        int client = accept(server_fd, NULL, NULL);
        if (client < 0) {
            perror("accept");
            continue;
        }
        
        char buf[256] = {0};
        int n = read(client, buf, sizeof(buf) - 1);
        if (n > 0) {
            printf("Received: %s", buf);
            write(client, "OK\n", 3);
        }
        close(client);
    }
    
    close(server_fd);
    return 0;
}
```

### 📄 클라이언트 예시 (`client.c`)
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket");
        return 1;
    }

    struct sockaddr_in server = {0};
    server.sin_family = AF_INET;
    server.sin_port = htons(8081);
    if (inet_pton(AF_INET, "127.0.0.1", &server.sin_addr) <= 0) {
        perror("inet_pton");
        close(sock);
        return 1;
    }

    if (connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
        perror("connect");
        close(sock);
        return 1;
    }

    char msg[256];
    printf("Enter message: ");
    if (fgets(msg, sizeof(msg), stdin) == NULL) {
        close(sock);
        return 1;
    }
    
    if (write(sock, msg, strlen(msg)) < 0) {
        perror("write");
        close(sock);
        return 1;
    }
    
    char response[256];
    int n = read(sock, response, sizeof(response) - 1);
    if (n > 0) {
        response[n] = '\0';
        printf("Server response: %s", response);
    }
    
    close(sock);
    return 0;
}
```

---

## 🧰 3단계 — 로그 관리 & 설정 파일 시스템

### 🎯 목표  
- 설정파일(`config.conf`)을 읽어 동작 파라미터를 적용하고,  
  프로그램 로그를 날짜별로 저장한다.

### 📄 config.conf 예시
```
LOG_PATH=./logs
INTERVAL=5
```

### 📄 코드 요약 (`main.c`)
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

char LOG_PATH[128];
int INTERVAL;

void load_config() {
    FILE *fp = fopen("config.conf", "r");
    if (fp == NULL) {
        fprintf(stderr, "Error: Cannot open config.conf\n");
        exit(1);
    }
    char key[64], val[64];
    while (fscanf(fp, "%[^=]=%s\n", key, val) != EOF) {
        if (strcmp(key, "LOG_PATH") == 0) strcpy(LOG_PATH, val);
        if (strcmp(key, "INTERVAL") == 0) INTERVAL = atoi(val);
    }
    fclose(fp);
}

void write_log(const char *msg) {
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    char filename[256];
    snprintf(filename, sizeof(filename), "%s/log_%04d%02d%02d.txt", LOG_PATH, tm->tm_year+1900, tm->tm_mon+1, tm->tm_mday);
    
    // Create directory if it doesn't exist
    struct stat st = {0};
    if (stat(LOG_PATH, &st) == -1) {
        mkdir(LOG_PATH, 0755);
    }
    
    FILE *fp = fopen(filename, "a");
    if (fp == NULL) {
        fprintf(stderr, "Error: Cannot open log file %s\n", filename);
        return;
    }
    fprintf(fp, "%02d:%02d:%02d %s\n", tm->tm_hour, tm->tm_min, tm->tm_sec, msg);
    fclose(fp);
    printf("%02d:%02d:%02d Log written: %s\n", tm->tm_hour, tm->tm_min, tm->tm_sec, msg);
}

int main() {
    load_config();
    printf("Logging service started. Log path: %s, Interval: %d seconds\n", LOG_PATH, INTERVAL);
    printf("Press Ctrl+C to stop.\n\n");
    while (1) {
        write_log("System OK");
        sleep(INTERVAL);
    }
}
```

---

## ⚡ 4단계 — 센서 시뮬레이터

### 🎯 목표  
- 가상의 센서 데이터를 생성하고 TCP 서버로 전송  
- 일정 주기마다 온도·전압·전류 등 난수 데이터를 송신

### 📄 코드 예시 (`sensor_sim.c`)

#### 포트 자동 할당 기능이 포함된 서버 코드

**주요 개념:**
- `AF_INET`: Address Family for IPv4 (값: 2, 정의: `/usr/include/bits/socket.h`)
- `SOCK_STREAM`: TCP 소켓 타입 (값: 1, 정의: `/usr/include/bits/socket_type.h`)
- 포트 8080이 점유 중이면 자동으로 다음 포트 검색 (8080~65535)

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <arpa/inet.h>
#include <sys/socket.h>      // socket(), AF_INET, SOCK_STREAM 등 정의
#include <netinet/in.h>      // struct sockaddr_in 정의
#include <errno.h>

/*
 * AF_INET: Address Family for IPv4
 *   - 정의: /usr/include/bits/socket.h
 *   - 값: 2 (PF_INET과 동일)
 *   - 의미: IPv4 인터넷 프로토콜 패밀리 사용
 *
 * SOCK_STREAM: Socket Type for TCP
 *   - 정의: /usr/include/bits/socket_type.h
 *   - 값: 1
 *   - 의미: 연결 지향적, 신뢰성 있는 바이트 스트림 (TCP)
 *   - 반대: SOCK_DGRAM (UDP, 값 2)
 *
 * socket() 함수:
 *   - 프로토타입: int socket(int domain, int type, int protocol);
 *   - domain: 주소 패밀리 (AF_INET = IPv4)
 *   - type: 소켓 타입 (SOCK_STREAM = TCP, SOCK_DGRAM = UDP)
 *   - protocol: 프로토콜 (0 = 자동 선택, TCP/UDP는 자동으로 선택됨)
 */

int main() {
    srand(time(NULL));
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket");
        return 1;
    }

    // SO_REUSEADDR 옵션 설정 (TIME_WAIT 상태 포트 재사용 가능)
    int opt = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt");
        close(sock);
        return 1;
    }

    struct sockaddr_in serv = {0};
    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = INADDR_ANY;
    
    int port;
    int found = 0;
    
    // 8080부터 시작해서 사용 가능한 포트 찾기
    for (port = 8080; port <= 65535; port++) {
        serv.sin_port = htons(port);
        errno = 0;  // errno 초기화
        if (bind(sock, (struct sockaddr*)&serv, sizeof(serv)) == 0) {
            found = 1;
            printf("Successfully bound to port %d\n", port);
            break;
        }
        // EADDRINUSE: 주소가 이미 사용 중인 경우 다음 포트 시도
        if (errno != EADDRINUSE) {
            perror("bind");
            close(sock);
            return 1;
        }
    }
    
    if (!found) {
        fprintf(stderr, "No available port found (8080-65535)\n");
        close(sock);
        return 1;
    }
    
    // listen: 클라이언트 연결 대기 큐 설정 (최대 5개)
    if (listen(sock, 5) < 0) {
        perror("listen");
        close(sock);
        return 1;
    }
    
    printf("Server listening on port %d...\n", port);
    
    // 클라이언트 연결을 받아서 센서 데이터 처리
    while (1) {
        int client = accept(sock, NULL, NULL);
        if (client < 0) {
            perror("accept");
            continue;
        }
        
        char buf[256] = {0};
        int n = read(client, buf, sizeof(buf) - 1);
        if (n > 0) {
            printf("Received sensor data: %s", buf);
            // 센서 데이터 처리 (로그 저장, 알림 등)
            write(client, "ACK\n", 4);
        }
        close(client);
    }
    
    close(sock);
    return 0;
}
```

#### 클라이언트 연결 예시 (`client.c`)

**개선 사항:**
- 필수 헤더 파일 추가 (`sys/socket.h`, `netinet/in.h`, `string.h`, `signal.h`)
- 모든 시스템 호출에 에러 처리 추가
- Ctrl+C 시 소켓 정리 (시그널 핸들러)
- `write()` 에러 체크 추가

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <signal.h>

int sock = -1;

void cleanup(int sig) {
    if (sock >= 0) {
        close(sock);
    }
    exit(0);
}

int main() {
    signal(SIGINT, cleanup);
    
    srand(time(NULL));
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket");
        return 1;
    }
    
    struct sockaddr_in serv = {0};
    serv.sin_family = AF_INET;
    serv.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &serv.sin_addr);

    if (connect(sock, (struct sockaddr*)&serv, sizeof(serv)) < 0) {
        perror("connect");
        close(sock);
        return 1;
    }

    printf("Connected to server. Sending sensor data...\n");
    while (1) {
        float temp = 20.0 + rand() % 100 / 10.0;
        float volt = 100 + rand() % 50;
        char buf[64];
        sprintf(buf, "TEMP=%.1f,VOLT=%.1f\n", temp, volt);
        
        if (write(sock, buf, strlen(buf)) < 0) {
            perror("write");
            break;
        }
        sleep(3);
    }
    
    close(sock);
    return 0;
}
```

---

## 🧱 마무리
이 1~4단계는 **실제 산업용 코드 구조**를 단순화한 버전입니다.  
직접 타이핑하며 컴파일하고, 각 단계에서  
- 함수 설계  
- 파일 경로  
- Makefile 수정  
- 로그·출력·네트워크 동작 확인  
을 경험하면, 겐바 투입 전에 충분히 실전 감각을 익힐 수 있습니다.
