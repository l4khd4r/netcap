#include <arpa/inet.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <net/if.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include "../include/ethernet.h"
#include "../include/protocol.h"
#include "../include/ipv4.h"
#include "../include/tcp.h"
#include "../include/udp.h"

#include <arpa/inet.h>

static void print_ipv4(const t_ipv4_header *header)
{
    printf("IPv4\n");
    printf("  Source:      %u.%u.%u.%u\n",
        header->src_ip[0], header->src_ip[1],
        header->src_ip[2], header->src_ip[3]);
    printf("  Destination: %u.%u.%u.%u\n",
        header->dest_ip[0], header->dest_ip[1],
        header->dest_ip[2], header->dest_ip[3]);
    printf("  TTL:         %u\n", header->ttl);
    printf("  Protocol:    %s (%u)\n", ipv4_protocol_name(header->protocol), header->protocol);
    printf("  Header len:  %u bytes\n", header->ihl * 4);
    printf("  Total len:   %u bytes\n", header->total_length);
}





int main(int argc, char **argv) {

    int fd;
    struct sockaddr_ll addr = {0};
    unsigned char buffer[66536];
    ssize_t length;

    if (argc != 2) {
        fprintf(stderr,  "Usage: %s <interface>\n" , argv[0]);
        return 1;
    }

    unsigned int ifindex = if_nametoindex(argv[1]);
    if (ifindex == 0) {
        perror("if_nametoindex");
        return 1;
    }


    fd  = socket(AF_PACKET  , SOCK_RAW , htons(ETH_P_ALL));

    if (fd == -1) {
        perror("socket");
        return 1;
    }

    addr.sll_family = AF_PACKET ;
    addr.sll_protocol = htons(ETH_P_ALL);
    addr.sll_ifindex = ifindex;

    if(bind(fd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        perror("bind");
        close(fd);
        return 1;
    }

    printf("Capturing on %s. Press Ctrl+C to stop.\n", argv[1]);

    while(1) {
        length = recvfrom(fd, buffer,  sizeof(buffer), 0 , NULL , NULL);
        if(length == -1 ) {
            perror("recvfrom");
            close(fd);
            return 1;
        }
        printf("Received packet of length %zd bytes\n", length);
        t_ethernet_header header;
        t_ipv4_header ipv4_header; 
        t_tcp_header tcp_header;
        t_udp_header udp_header;
        if (ethernet_parse(buffer, length, &header) == -1) {
            fprintf(stderr, "Failed to parse Ethernet header\n");
        }
        print_ethernet(&header);

        if(header.ethertype != 0x0800)
            continue; // Not an IPv4 packet

        if(ipv4_parse(buffer + ETHERNET_HEADER_LEN, length - ETHERNET_HEADER_LEN, &ipv4_header) == -1) {
            fprintf(stderr, "Failed to parse IPv4 header\n");
            continue;
        }
        print_ipv4(&ipv4_header);
        if(ipv4_header.protocol == 17) {
            if(udp_parse(buffer + ETHERNET_HEADER_LEN + (ipv4_header.ihl * 4), length - ETHERNET_HEADER_LEN - (ipv4_header.ihl * 4), &udp_header) == -1) {
                fprintf(stderr, "Failed to parse UDP header\n");
                continue ;
            }
            print_udp(&udp_header);
        }


        if(ipv4_header.protocol != 6)
            continue; // Not a TCP packet

        if(tcp_parse(buffer + ETHERNET_HEADER_LEN + (ipv4_header.ihl * 4), length - ETHERNET_HEADER_LEN - (ipv4_header.ihl * 4), &tcp_header) == -1) {
            fprintf(stderr, "Failed to parse TCP header\n");
            continue ;
        }



        print_tcp(&tcp_header);
    }


    close(fd);
    return 0 ;
}