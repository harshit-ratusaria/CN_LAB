/*
 * UDP Fruit Client
 * Sends messages to server in the form "fruit quantity" and waits for a reply.
 *
 * Compile:
 *   gcc client.c -o client
 * Run:
 *   ./client <server-ip>  (server-ip optional, default 127.0.0.1)
 *
 * Example:
 *   ./client 10.0.0.3
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 5000
#define BUFFER_SIZE 2048

int main(int argc, char *argv[]) {
    int sockfd;
    struct sockaddr_in servaddr;
    char buffer[BUFFER_SIZE];

    const char *server_ip = "10.0.0.1";
    if (argc >= 2) server_ip = argv[1];

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, server_ip, &servaddr.sin_addr) <= 0) {
        fprintf(stderr, "Invalid server IP: %s\n", server_ip);
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("[UDP CLIENT] Sending to %s:%d\n", server_ip, PORT);

    while (1) {
        char fruit[32];
        int qty;

        printf("Enter fruit name (or 'exit' to quit): ");
        if (scanf("%31s", fruit) != 1) break;
        if (strcmp(fruit, "exit") == 0) break;

        printf("Enter quantity: ");
        if (scanf("%d", &qty) != 1) {
            // clear stdin
            int c; while ((c = getchar()) != '\n' && c != EOF);
            printf("Invalid quantity. Try again.\n");
            continue;
        }

        snprintf(buffer, sizeof(buffer), "%s %d", fruit, qty);

        ssize_t sent = sendto(sockfd, buffer, strlen(buffer), 0,
                              (const struct sockaddr *) &servaddr, sizeof(servaddr));
        if (sent < 0) {
            perror("sendto failed");
            continue;
        }

        // Wait for server reply
        struct sockaddr_in from;
        socklen_t fromlen = sizeof(from);
        ssize_t n = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0,
                             (struct sockaddr *) &from, &fromlen);
        if (n < 0) {
            perror("recvfrom");
            continue;
        }
        buffer[n] = '\0';
        printf("\n--- Server response ---\n%s\n------------------------\n", buffer);
    }

    close(sockfd);
    printf("Client exiting.\n");
    return 0;
}

