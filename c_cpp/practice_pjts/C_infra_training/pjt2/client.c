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