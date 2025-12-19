#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

int client_sockets[MAX_CLIENTS];
int client_count = 0;
pthread_mutex_t lock;
FILE *log_file;

void broadcast(char *message, int sender_sock)
{
    pthread_mutex_lock(&lock);

    for (int i = 0; i < client_count; i++)
    {
        if (client_sockets[i] != sender_sock)
        {
            send(client_sockets[i], message, strlen(message), 0);
        }
    }

    pthread_mutex_unlock(&lock);
}

void *handle_client(void *arg)
{
    int client_sock = *((int *)arg);
    char buffer[BUFFER_SIZE];

    while (1)
    {
        int bytes = recv(client_sock, buffer, sizeof(buffer), 0);
        if (bytes <= 0)
        {
            close(client_sock);
            break;
        }

        buffer[bytes] = '\0';

        // Write to log file
        pthread_mutex_lock(&lock);
        fprintf(log_file, "%s", buffer);
        fflush(log_file);
        pthread_mutex_unlock(&lock);

        // Broadcast message
        broadcast(buffer, client_sock);
    }

    return NULL;
}

int main()
{
    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;
    pthread_t tid;

    pthread_mutex_init(&lock, NULL);
    log_file = fopen("chat_log.txt", "a");

    server_sock = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(5555);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(server_sock, MAX_CLIENTS);

    printf("Chat Server started on port 5555...\n");

    while (1)
    {
        addr_size = sizeof(client_addr);
        client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &addr_size);

        pthread_mutex_lock(&lock);
        client_sockets[client_count++] = client_sock;
        pthread_mutex_unlock(&lock);

        pthread_create(&tid, NULL, handle_client, &client_sock);
    }

    fclose(log_file);
    close(server_sock);
    return 0;
}
