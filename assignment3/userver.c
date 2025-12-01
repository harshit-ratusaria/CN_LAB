/*
 * UDP Fruit Server
 * Listens on PORT (default 5000). Receives messages in form:
 *   fruit_name quantity
 * Replies with success/regret + remaining stock + last-sold timestamp
 * Also maintains and sends the list of unique customers (<IP,port>) and count.
 *
 * Compile:
 *   gcc server.c -o server
 * Run:
 *   ./server
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 5000
#define BUFFER_SIZE 1024
#define MAX_FRUITS 10
#define MAX_CUSTOMERS 100

typedef struct {
    char name[32];
    int quantity;
    char last_sold[64];
} Fruit;

typedef struct {
    struct sockaddr_in addr;
    int used;
} CustomerEntry;

Fruit fruits[MAX_FRUITS];
int fruit_count = 0;

CustomerEntry customers[MAX_CUSTOMERS];
int customer_count = 0;

/* Compare sockaddr_in (IP + port) for equality */
int same_addr(struct sockaddr_in *a, struct sockaddr_in *b) {
    return (a->sin_addr.s_addr == b->sin_addr.s_addr) && (a->sin_port == b->sin_port);
}

/* Add customer if unique. Return 1 if added, 0 if already present. */
int add_unique_customer(struct sockaddr_in *cliaddr) {
    for (int i = 0; i < customer_count; ++i) {
        if (same_addr(&(customers[i].addr), cliaddr)) return 0;
    }
    if (customer_count < MAX_CUSTOMERS) {
        customers[customer_count].addr = *cliaddr;
        customers[customer_count].used = 1;
        customer_count++;
        return 1;
    }
    return 0;
}

void init_fruits() {
    strcpy(fruits[0].name, "apple");
    fruits[0].quantity = 10;
    strcpy(fruits[0].last_sold, "Never");

    strcpy(fruits[1].name, "banana");
    fruits[1].quantity = 15;
    strcpy(fruits[1].last_sold, "Never");

    strcpy(fruits[2].name, "mango");
    fruits[2].quantity = 20;
    strcpy(fruits[2].last_sold, "Never");

    fruit_count = 3;
}

void timestamp_now(char *buf, size_t len) {
    time_t now = time(NULL);
    struct tm *tmv = localtime(&now);
    strftime(buf, len, "%Y-%m-%d %H:%M:%S", tmv);
}

int find_fruit_index(const char *name) {
    for (int i = 0; i < fruit_count; ++i) {
        if (strcasecmp(name, fruits[i].name) == 0) return i;
    }
    return -1;
}

int main() {
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;
    char buffer[BUFFER_SIZE];

    init_fruits();

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    servaddr.sin_family = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("[UDP SERVER] Listening on port %d\n", PORT);

    while (1) {
        socklen_t len = sizeof(cliaddr);
        ssize_t n = recvfrom(sockfd, (char *)buffer, BUFFER_SIZE - 1, 0,
                             (struct sockaddr *) &cliaddr, &len);
        if (n < 0) {
            perror("recvfrom");
            continue;
        }
        buffer[n] = '\0';

        // Parse input: expected "fruit qty"
        char fruit_name[32];
        int qty = 0;
        int items = sscanf(buffer, "%31s %d", fruit_name, &qty);
        char response[BUFFER_SIZE];
        memset(response, 0, sizeof(response));

        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(cliaddr.sin_addr), client_ip, INET_ADDRSTRLEN);
        int client_port = ntohs(cliaddr.sin_port);

        printf("[REQ] %s:%d -> '%s'\n", client_ip, client_port, buffer);

        if (items != 2 || qty <= 0) {
            snprintf(response, sizeof(response),
                     "Invalid request format. Use: <fruit> <quantity> (quantity > 0)\n");
            sendto(sockfd, response, strlen(response), 0,
                   (const struct sockaddr *) &cliaddr, len);
            continue;
        }

        int idx = find_fruit_index(fruit_name);
        if (idx < 0) {
            snprintf(response, sizeof(response), "Fruit '%s' not available.\n", fruit_name);
        } else {
            if (fruits[idx].quantity >= qty) {
                fruits[idx].quantity -= qty;
                timestamp_now(fruits[idx].last_sold, sizeof(fruits[idx].last_sold));
                snprintf(response, sizeof(response),
                         "Purchase successful: %d %s(s) bought.\n"
                         "Remaining stock: %d\n"
                         "Last sold at: %s\n",
                         qty, fruits[idx].name, fruits[idx].quantity, fruits[idx].last_sold);
            } else {
                snprintf(response, sizeof(response),
                         "Sorry, only %d %s(s) available. Purchase NOT completed.\n",
                         fruits[idx].quantity, fruits[idx].name);
            }
        }

        // track unique customers
        add_unique_customer(&cliaddr);

        // append customer info
        char customer_info[BUFFER_SIZE];
        int pos = 0;
        pos += snprintf(customer_info + pos, sizeof(customer_info) - pos,
                        "Unique customers so far: %d\nCustomer list:\n", customer_count);

        for (int i = 0; i < customer_count; ++i) {
            char ipbuf[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &(customers[i].addr.sin_addr), ipbuf, INET_ADDRSTRLEN);
            int port = ntohs(customers[i].addr.sin_port);
            pos += snprintf(customer_info + pos, sizeof(customer_info) - pos,
                            "%s:%d\n", ipbuf, port);
            if (pos >= (int)sizeof(customer_info) - 100) break; // safety
        }

        // Combine and send
        strncat(response, "\n", sizeof(response) - strlen(response) - 1);
        strncat(response, customer_info, sizeof(response) - strlen(response) - 1);

        ssize_t sent = sendto(sockfd, response, strlen(response), 0,
                              (const struct sockaddr *) &cliaddr, len);
        if (sent < 0) {
            perror("sendto");
        }
    }

    close(sockfd);
    return 0;
}

