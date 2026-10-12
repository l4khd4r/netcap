
#ifndef IPV4_H
#define IPV4_H

#include <stdint.h>
#include <stddef.h>

#define IPV4_MIN_HEADER_LEN 20
#define IPV4_ADDR_LEN 4

typedef struct ipv4_header
{
    uint8_t version;
    uint8_t ihl;
    uint16_t total_length;
    uint8_t ttl;
    uint8_t protocol;
    uint8_t src_ip[IPV4_ADDR_LEN];
    uint8_t dest_ip[IPV4_ADDR_LEN];
    uint8_t flags;
    uint8_t fragment_offset;
} t_ipv4_header;

int ipv4_parse(const uint8_t *data, size_t length,
        t_ipv4_header *header);

#endif
