#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <time.h>

#define PORT 5000
#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

// Fruit record structure
typedef struct {
    char name[20];
    int quantity;
    char last_sold[64];
} Fruit;

Fruit fruits[] = {
    {"apple", 10, "Never"},
    {"banana", 15, "Never"},
    {"mango", 20, "Never"}
};
int fruit_count = 3;

// Store unique customers
struct sockaddr_in customers[MAX_CLIENTS];
int customer_count = 0;

pthread_mutex_t lock;

int is_unique_customer(struct sockaddr_in addr) {
    for (int i = 0; i < customer_count; i++) {
        if (customers[i].sin_addr.s_addr == addr.sin_addr.s_addr &&
            customers[i].sin_port == addr.sin_port) {
            return 0; // already present
        }
    }
    customers[customer_count++] = addr;
    return 1;
}

void *handle_client(void *arg) {
    int sock = *(int*)arg;
    free(arg);

    char buffer[BUFFER_SIZE];
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);
    getpeername(sock, (struct sockaddr*)&addr, &addr_len);

    while (1) {
        memset(buffer, 0, sizeof(buffer));
        int bytes = recv(sock, buffer, sizeof(buffer), 0);
        if (bytes <= 0) break;

        char fruit_name[20];
        int qty;
        sscanf(buffer, "%s %d", fruit_name, &qty);

        char response[BUFFER_SIZE];
        memset(response, 0, sizeof(response));

        pthread_mutex_lock(&lock);

        int found = 0;
        for (int i = 0; i < fruit_count; i++) {
            if (strcasecmp(fruit_name, fruits[i].name) == 0) {
                found = 1;
                if (fruits[i].quantity >= qty) {
                    fruits[i].quantity -= qty;

                    time_t now = time(NULL);
                    strftime(fruits[i].last_sold, sizeof(fruits[i].last_sold),
                             "%Y-%m-%d %H:%M:%S", localtime(&now));

                    snprintf(response, sizeof(response),
                        "Purchase successful: %d %s(s) bought.\n"
                        "Remaining stock: %d\n"
                        "Last sold at: %s\n",
                        qty, fruits[i].name, fruits[i].quantity, fruits[i].last_sold);
                } else {
                    snprintf(response, sizeof(response),
                        "Sorry, only %d %s(s) available.\n",
                        fruits[i].quantity, fruits[i].name);
                }
                break;
            }
        }

        if (!found) {
            snprintf(response, sizeof(response), "Fruit '%s' not available.\n", fruit_name);
        }

        // Track unique customer
        is_unique_customer(addr);

        // Append customer info
        char customer_info[BUFFER_SIZE];
        snprintf(customer_info, sizeof(customer_info),
                 "Unique customers so far: %d\nCustomer list:\n",
                 customer_count);

        for (int i = 0; i < customer_count; i++) {
            char ip[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &(customers[i].sin_addr), ip, sizeof(ip));
            char entry[128];
            snprintf(entry, sizeof(entry), "%s:%d\n", ip, ntohs(customers[i].sin_port));
            strcat(customer_info, entry);
        }

        strcat(response, customer_info);
        pthread_mutex_unlock(&lock);

        send(sock, response, strlen(response), 0);
    }

    close(sock);
    return NULL;
}

int main() {
    int server_fd, new_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;

    pthread_mutex_init(&lock, NULL);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Socket failed");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(1);
    }

    if (listen(server_fd, 5) < 0) {
        perror("Listen failed");
        exit(1);
    }

    printf("[SERVER STARTED] Listening on port %d...\n", PORT);

    while (1) {
        addr_size = sizeof(client_addr);
        new_sock = accept(server_fd, (struct sockaddr*)&client_addr, &addr_size);
        if (new_sock < 0) {
            perror("Accept failed");
            continue;
        }

        printf("[NEW CONNECTION] %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        int *pclient = malloc(sizeof(int));
        *pclient = new_sock;
        pthread_t t;
        pthread_create(&t, NULL, handle_client, pclient);
        pthread_detach(t);
    }

    close(server_fd);
    pthread_mutex_destroy(&lock);
    return 0;
}

