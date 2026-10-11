#ifndef STATS_H
# define STATS_H

# include <stdint.h>

typedef struct s_stats
{
    uint64_t total_packets;
    uint64_t ipv4_packets;
    uint64_t tcp_packets;
    uint64_t udp_packets;
    uint64_t icmp_packets;
    uint64_t other_packets;
    uint64_t malformed_packets;
}   t_stats;

void    stats_init(t_stats *stats);
void    stats_print(const t_stats *stats);

#endif