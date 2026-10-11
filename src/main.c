#include <arpa/inet.h>
#include <errno.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <net/if.h>
#include <signal.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>

#include "../include/ethernet.h"
#include "../include/ipv4.h"
#include "../include/protocol.h"
#include "../include/tcp.h"
#include "../include/udp.h"
#include "../include/icmp.h"
#include "../include/stats.h"

static volatile sig_atomic_t g_running = 1;

static void handle_sigint(int signal_number)
{
    (void)signal_number;
    g_running = 0;
}

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
    printf("  Protocol:    %s (%u)\n",
        ipv4_protocol_name(header->protocol), header->protocol);
    printf("  Header len:  %u bytes\n", header->ihl * 4);
    printf("  Total len:   %u bytes\n", header->total_length);
}

static void process_packet(const unsigned char *buffer,
    size_t length, t_stats *stats)
{
    t_ethernet_header eth;
    t_ipv4_header ip;
    size_t ip_available;
    size_t ip_header_len;
    size_t payload_len;
    const unsigned char *payload;

    if (ethernet_parse(buffer, length, &eth) == -1)
    {
        fprintf(stderr, "Failed to parse Ethernet header\n");
        stats->malformed_packets++;
        return;
    }

    print_ethernet(&eth);

    if (eth.ethertype != ETH_P_IP)
    {
        stats->other_packets++;
        return;
    }

    if (length < ETHERNET_HEADER_LEN)
    {
        stats->malformed_packets++;
        return;
    }

    ip_available = length - ETHERNET_HEADER_LEN;

    if (ipv4_parse(buffer + ETHERNET_HEADER_LEN,
            ip_available, &ip) == -1)
    {
        fprintf(stderr, "Failed to parse IPv4 header\n");
        stats->malformed_packets++;
        return;
    }

    ip_header_len = ip.ihl * 4;

    if (ip.total_length > ip_available ||
        ip.total_length < ip_header_len)
    {
        fprintf(stderr, "Invalid or truncated IPv4 packet\n");
        stats->malformed_packets++;
        return;
    }

    stats->ipv4_packets++;
    print_ipv4(&ip);

    payload = buffer + ETHERNET_HEADER_LEN + ip_header_len;
    payload_len = ip.total_length - ip_header_len;

    switch (ip.protocol)
    {
        case IPPROTO_TCP:
        {
            t_tcp_header tcp;

            if (tcp_parse(payload, payload_len, &tcp) == -1)
            {
                fprintf(stderr, "Failed to parse TCP header\n");
                stats->malformed_packets++;
                return;
            }
            stats->tcp_packets++;
            print_tcp(&tcp);
            break;
        }

        case IPPROTO_UDP:
        {
            t_udp_header udp;

            if (udp_parse(payload, payload_len, &udp) == -1)
            {
                fprintf(stderr, "Failed to parse UDP header\n");
                stats->malformed_packets++;
                return;
            }
            stats->udp_packets++;
            print_udp(&udp);
            break;
        }

        case IPPROTO_ICMP:
        {
            t_icmp_header icmp;

            if (icmp_parse(payload, payload_len, &icmp) == -1)
            {
                fprintf(stderr, "Failed to parse ICMP header\n");
                stats->malformed_packets++;
                return;
            }
            stats->icmp_packets++;
            print_icmp(&icmp);
            break;
        }

        default:
            stats->other_packets++;
            printf("Unsupported IPv4 protocol: %u\n", ip.protocol);
            break;
    }
}

int main(int argc, char **argv)
{
    int fd;
    unsigned int ifindex;
    struct sockaddr_ll addr = {0};
    unsigned char buffer[66536];
    ssize_t length;
    t_stats stats;

    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <interface>\n", argv[0]);
        return 1;
    }

    ifindex = if_nametoindex(argv[1]);
    if (ifindex == 0)
    {
        perror("if_nametoindex");
        return 1;
    }

    fd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (fd == -1)
    {
        perror("socket");
        return 1;
    }

    addr.sll_family = AF_PACKET;
    addr.sll_protocol = htons(ETH_P_ALL);
    addr.sll_ifindex = (int)ifindex;

    if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
    {
        perror("bind");
        close(fd);
        return 1;
    }

    signal(SIGINT, handle_sigint);
    stats_init(&stats);

    printf("Capturing on %s. Press Ctrl+C to stop.\n", argv[1]);

    while (g_running)
    {
        length = recvfrom(fd, buffer, sizeof(buffer), 0, NULL, NULL);

        if (length == -1)
        {
            if (errno == EINTR && !g_running)
                break;
            if (errno == EINTR)
                continue;
            perror("recvfrom");
            break;
        }

        stats.total_packets++;
        process_packet(buffer, (size_t)length, &stats);
    }

    close(fd);
    stats_print(&stats);
    return 0;
}
