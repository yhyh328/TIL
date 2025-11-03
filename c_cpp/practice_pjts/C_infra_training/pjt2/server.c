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