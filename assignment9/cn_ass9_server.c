#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 9000
#define BUF 1024

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    char buffer[BUF];
    char filename[100];
    long filesize, received = 0;
    int bytes;
    FILE *fp;

    /* Create socket */
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Socket");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    listen(server_fd, 5);

    printf("Server listening...\n");

    client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &addr_len);
    printf("Client connected\n");

    /* ===== RECEIVE HEADER ===== */
    memset(buffer, 0, BUF);
    int i = 0;
    char ch;

    while (i < BUF - 1) {
        if (recv(client_fd, &ch, 1, 0) <= 0)
            break;
        if (ch == '\n')
            break;
        buffer[i++] = ch;
    }
    buffer[i] = '\0';

    sscanf(buffer, "%s %ld", filename, &filesize);

    printf("Receiving %s (%ld bytes)\n", filename, filesize);

    fp = fopen(filename, "wb");
    if (!fp) {
        perror("File");
        exit(1);
    }

    /* ===== RECEIVE FILE DATA ===== */
    received = 0;
    while (received < filesize) {
        bytes = recv(client_fd, buffer, BUF, 0);

        if (bytes <= 0)
            break;

        fwrite(buffer, 1, bytes, fp);
        received += bytes;
    }

    fclose(fp);
    close(client_fd);
    close(server_fd);

    printf("File received successfully\n");
    return 0;
}
