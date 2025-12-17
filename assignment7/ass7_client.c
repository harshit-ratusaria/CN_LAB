#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080
#define BUF_SIZE 1024

int main() {
    int sockfd;
    char buffer[BUF_SIZE];
    struct sockaddr_in server_addr;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "10.0.0.1", &server_addr.sin_addr); // server IP

    printf("Enter operation (e.g., + 5 3 | sin 90 0): ");
    fgets(buffer, BUF_SIZE, stdin);

    sendto(sockfd, buffer, strlen(buffer), 0,
           (struct sockaddr *)&server_addr, sizeof(server_addr));
    printf("Waiting for server response...\n");
    fflush(stdout);

    memset(buffer, 0, BUF_SIZE);

    recvfrom(sockfd, buffer, BUF_SIZE, 0, NULL, NULL);
    printf("Server Response: %s\n", buffer);

    close(sockfd);
    return 0;
}

