#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 5000
#define BUFFER_SIZE 1024

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("10.0.0.1"); // change for Mininet server IP

    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        exit(1);
    }

    printf("[CONNECTED TO SERVER]\n");

    while (1) {
        char fruit[20];
        int qty;

        printf("Enter fruit name (or 'exit' to quit): ");
        scanf("%s", fruit);
        if (strcmp(fruit, "exit") == 0) break;

        printf("Enter quantity: ");
        scanf("%d", &qty);

        snprintf(buffer, sizeof(buffer), "%s %d", fruit, qty);
        send(sock, buffer, strlen(buffer), 0);

        memset(buffer, 0, sizeof(buffer));
        int bytes = recv(sock, buffer, sizeof(buffer), 0);
        if (bytes > 0) {
            buffer[bytes] = '\0';
            printf("\n--- Server Response ---\n%s\n", buffer);
        }
    }

    close(sock);
    return 0;
}

