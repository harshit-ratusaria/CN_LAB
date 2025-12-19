#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

int sock;
char name[50];

void *send_msg(void *arg)
{
    char buffer[BUFFER_SIZE];

    while (1)
    {
        fgets(buffer, BUFFER_SIZE, stdin);

        char message[BUFFER_SIZE];
        snprintf(message, BUFFER_SIZE, "%s: %s", name, buffer);

        send(sock, message, strlen(message), 0);
    }
}

void *recv_msg(void *arg)
{
    char buffer[BUFFER_SIZE];

    while (1)
    {
        int bytes = recv(sock, buffer, BUFFER_SIZE, 0);
        if (bytes > 0)
        {
            buffer[bytes] = '\0';
            printf("%s", buffer);
            fflush(stdout);
        }
    }
}

int main()
{
    struct sockaddr_in server_addr;
    pthread_t send_thread, recv_thread;

    printf("Enter your name: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = 0;

    sock = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(5555);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));

    printf("Connected to chat server...\n");

    pthread_create(&send_thread, NULL, send_msg, NULL);
    pthread_create(&recv_thread, NULL, recv_msg, NULL);

    pthread_join(send_thread, NULL);
    pthread_join(recv_thread, NULL);

    close(sock);
    return 0;
}
