#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/if_ether.h>
#include <linux/if_packet.h>

#define BUFFER_SIZE 65536

int main() {
    int sock_raw;
    unsigned char buffer[BUFFER_SIZE];

    sock_raw = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (sock_raw < 0) {
        perror("Socket Error");
        return 1;
    }

    printf("TCP Traffic Analyzer Started...\n");

    while (1) {
        int data_size = recvfrom(sock_raw, buffer, BUFFER_SIZE, 0, NULL, NULL);
        if (data_size < 0) {
            perror("Recvfrom error");
            return 1;
        }

        struct ethhdr *eth = (struct ethhdr *)buffer;

        // Check if IP packet
        if (ntohs(eth->h_proto) == ETH_P_IP) {

            struct iphdr *ip = (struct iphdr *)(buffer + sizeof(struct ethhdr));

            // Check if TCP packet
            if (ip->protocol == IPPROTO_TCP) {

                unsigned short iphdrlen = ip->ihl * 4;
                struct tcphdr *tcp =
                    (struct tcphdr *)(buffer + sizeof(struct ethhdr) + iphdrlen);

                struct sockaddr_in src, dest;
                memset(&src, 0, sizeof(src));
                memset(&dest, 0, sizeof(dest));

                src.sin_addr.s_addr = ip->saddr;
                dest.sin_addr.s_addr = ip->daddr;

                printf("\n===== TCP Packet =====\n");
                printf("Source IP       : %s\n", inet_ntoa(src.sin_addr));
                printf("Destination IP  : %s\n", inet_ntoa(dest.sin_addr));
                printf("Source Port     : %u\n", ntohs(tcp->source));
                printf("Destination Port: %u\n", ntohs(tcp->dest));
                printf("Sequence Number : %u\n", ntohl(tcp->seq));
                printf("Ack Number      : %u\n", ntohl(tcp->ack_seq));
                printf("Header Length   : %d Bytes\n", tcp->doff * 4);

                printf("Flags           : ");
                if (tcp->syn) printf("SYN ");
                if (tcp->ack) printf("ACK ");
                if (tcp->fin) printf("FIN ");
                if (tcp->psh) printf("PSH ");
                if (tcp->rst) printf("RST ");
                if (tcp->urg) printf("URG ");
                printf("\n");
            }
        }
    }

    close(sock_raw);
    return 0;
}

