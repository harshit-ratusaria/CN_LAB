#include <pcap.h>
#include <stdio.h>
#include <netinet/ip.h>
#include <netinet/if_ether.h>
#include <netinet/ip_icmp.h>

void packet_handler(u_char *args,
                     const struct pcap_pkthdr *header,
                     const u_char *packet)
{
    struct ether_header *eth;
    struct ip *ip_hdr;
    struct icmphdr *icmp_hdr;

    printf("\nTime: %ld.%06ld",
           header->ts.tv_sec,
           header->ts.tv_usec);

    eth = (struct ether_header *) packet;
    printf("\nL2: Ethernet | Type: 0x%04x",
           ntohs(eth->ether_type));

    if (ntohs(eth->ether_type) == ETHERTYPE_IP) {
        ip_hdr = (struct ip *)(packet + sizeof(struct ether_header));
        printf("\nL3: IP | Src: %s | Dst: %s",
               inet_ntoa(ip_hdr->ip_src),
               inet_ntoa(ip_hdr->ip_dst));

        if (ip_hdr->ip_p == IPPROTO_ICMP) {
            icmp_hdr = (struct icmphdr *)
              (packet + sizeof(struct ether_header) +
               (ip_hdr->ip_hl * 4));

            printf("\nL4: ICMP | Type: %d | Code: %d",
                   icmp_hdr->type,
                   icmp_hdr->code);
        }
    }
    printf("\n--------------------------------");
}

int main()
{
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle;

    handle = pcap_open_offline("ping_root_capture.pcap", errbuf);
    if (handle == NULL) {
        printf("Error opening pcap file: %s\n", errbuf);
        return 1;
    }

    pcap_loop(handle, 0, packet_handler, NULL);
    pcap_close(handle);
    return 0;
}
