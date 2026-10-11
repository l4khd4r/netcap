#include "../include/stats.h"
#include <stdio.h>

void    stats_init(t_stats *stats)
{
    if (!stats)
        return;
    stats->total_packets = 0;
    stats->ipv4_packets = 0;
    stats->tcp_packets = 0;
    stats->udp_packets = 0;
    stats->icmp_packets = 0;
    stats->other_packets = 0;
    stats->malformed_packets = 0;
}

void    stats_print(const t_stats *stats)
{
    if (!stats)
        return;
    printf("\n========== Capture Statistics ==========\n");
    printf("Total packets:      %llu\n",
        (unsigned long long)stats->total_packets);
    printf("IPv4 packets:       %llu\n",
        (unsigned long long)stats->ipv4_packets);
    printf("TCP packets:        %llu\n",
        (unsigned long long)stats->tcp_packets);
    printf("UDP packets:        %llu\n",
        (unsigned long long)stats->udp_packets);
    printf("ICMP packets:       %llu\n",
        (unsigned long long)stats->icmp_packets);
    printf("Other packets:      %llu\n",
        (unsigned long long)stats->other_packets);
    printf("Malformed packets:  %llu\n",
        (unsigned long long)stats->malformed_packets);
    printf("=========================================\n");
}