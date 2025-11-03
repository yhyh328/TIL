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

// Ctrl+C 시 소켓 정리
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