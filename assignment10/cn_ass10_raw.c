#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>

#define DEST_IP "8.8.8.8"   // target IP
#define DEST_PORT 80

// checksum function
unsigned short checksum(unsigned short *ptr, int nbytes)
{
    long sum = 0;
    unsigned short oddbyte;
    unsigned short answer;

    while (nbytes > 1) {
        sum += *ptr++;
        nbytes -= 2;
    }

    if (nbytes == 1) {
        oddbyte = 0;
        *((u_char *)&oddbyte) = *(u_char *)ptr;
        sum += oddbyte;
    }

    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    answer = (unsigned short)~sum;
    return answer;
}

int main()
{
    int sock;
    char packet[4096];

    struct iphdr *iph = (struct iphdr *)packet;
    struct tcphdr *tcph = (struct tcphdr *)(packet + sizeof(struct iphdr));
    char *payload = packet + sizeof(struct iphdr) + sizeof(struct tcphdr);

    char rollno[] = "ROLLNUMBER_12345";   // <-- replace with your roll number

    struct sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(DEST_PORT);
    sin.sin_addr.s_addr = inet_addr(DEST_IP);

    memset(packet, 0, 4096);

    // IP header
    iph->ihl = 5;
    iph->version = 4;
    iph->tos = 0;
    iph->tot_len = htons(sizeof(struct iphdr) + sizeof(struct tcphdr) + strlen(rollno));
    iph->id = htons(54321);
    iph->ttl = 64;
    iph->protocol = IPPROTO_TCP;
    iph->saddr = inet_addr("192.168.1.100"); // fake source
    iph->daddr = sin.sin_addr.s_addr;

    iph->check = checksum((unsigned short *)iph, sizeof(struct iphdr));

    // TCP header
    tcph->source = htons(12345);
    tcph->dest = htons(DEST_PORT);
    tcph->seq = htonl(1);
    tcph->ack_seq = 0;
    tcph->doff = 5;
    tcph->syn = 1;
    tcph->window = htons(5840);
    tcph->check = 0;
    tcph->urg_ptr = 0;

    strcpy(payload, rollno);

    // Create raw socket
    sock = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
    if (sock < 0) {
        perror("Socket error");
        return 1;
    }

    int one = 1;
    setsockopt(sock, IPPROTO_IP, IP_HDRINCL, &one, sizeof(one));

    sendto(sock, packet, ntohs(iph->tot_len), 0,
           (struct sockaddr *)&sin, sizeof(sin));

    printf("TCP packet sent with payload: %s\n", rollno);

    close(sock);
    return 0;
}
