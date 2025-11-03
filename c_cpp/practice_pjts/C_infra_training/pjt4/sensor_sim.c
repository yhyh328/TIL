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