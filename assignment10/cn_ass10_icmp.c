#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/ip_icmp.h>

unsigned short checksum(void *b, int len)
{
    unsigned short *buf = b;
    unsigned int sum = 0;
    unsigned short result;

    for (sum = 0; len > 1; len -= 2)
        sum += *buf++;

    if (len == 1)
        sum += *(unsigned char *)buf;

    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    result = ~sum;
    return result;
}

int main()
{
    int sock;
    struct sockaddr_in addr;
    char packet[64];

    struct icmphdr *icmp = (struct icmphdr *)packet;

    sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sock < 0) {
        perror("Socket error");
        return 1;
    }

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("8.8.8.8");  // target IP

    memset(packet, 0, sizeof(packet));

    icmp->type = ICMP_TIMESTAMP;      // Type 13
    icmp->code = 0;
    icmp->un.echo.id = htons(1234);
    icmp->un.echo.sequence = htons(1);
    icmp->checksum = 0;

    icmp->checksum = checksum(packet, sizeof(packet));

    sendto(sock, packet, sizeof(packet), 0,
           (struct sockaddr *)&addr, sizeof(addr));

    printf("ICMP Timestamp Request sent\n");

    close(sock);
    return 0;
}
