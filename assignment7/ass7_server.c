#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080
#define BUF_SIZE 1024

int main() {
    int sockfd;
    char buffer[BUF_SIZE];
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    printf("UDP Scientific Calculator Server running...\n");

    while (1) {
        memset(buffer, 0, BUF_SIZE);

        recvfrom(sockfd, buffer, BUF_SIZE, 0,
                 (struct sockaddr *)&client_addr, &addr_len);
        printf("Received: %s\n", buffer);

        fflush(stdout);

        char op[10];
        double a, b, result = 0;

        sscanf(buffer, "%s %lf %lf", op, &a, &b);

        if (strcmp(op, "+") == 0)
            result = a + b;
        else if (strcmp(op, "-") == 0)
            result = a - b;
        else if (strcmp(op, "*") == 0)
            result = a * b;
        else if (strcmp(op, "/") == 0)
            result = (b != 0) ? a / b : 0;
        else if (strcmp(op, "sin") == 0)
            result = sin(a * M_PI / 180);
        else if (strcmp(op, "cos") == 0)
            result = cos(a * M_PI / 180);
        else if (strcmp(op, "inv") == 0)
            result = (a != 0) ? 1 / a : 0;
        else
            strcpy(buffer, "Invalid Operation");

        snprintf(buffer, BUF_SIZE, "Result = %.4lf", result);

        sendto(sockfd, buffer, strlen(buffer), 0,
               (struct sockaddr *)&client_addr, addr_len);
    }

    close(sockfd);
    return 0;
}

