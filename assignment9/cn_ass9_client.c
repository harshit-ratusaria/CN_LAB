#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 9000
#define BUF 1024

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[BUF];
    char filename[100];
    FILE *fp;
    long filesize;
    int bytes;

    /* Create socket */
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("10.89.1.127"); // SERVER IP

    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connect");
        exit(1);
    }

    printf("Enter file name to upload: ");
    scanf("%s", filename);

    fp = fopen(filename, "rb");
    if (!fp) {
        perror("File");
        exit(1);
    }

    fseek(fp, 0, SEEK_END);
    filesize = ftell(fp);
    rewind(fp);

    /* ===== SEND HEADER ===== */
    snprintf(buffer, BUF, "%s %ld\n", filename, filesize);
    send(sock, buffer, strlen(buffer), 0);

    /* ===== SEND FILE DATA ===== */
    while ((bytes = fread(buffer, 1, BUF, fp)) > 0) {
        send(sock, buffer, bytes, 0);
    }

    fclose(fp);
    close(sock);

    printf("File uploaded successfully\n");
    return 0;
}
