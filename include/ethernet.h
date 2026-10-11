
#ifndef ETHERNET_H
#define ETHERNET_H

#include <stdint.h>
#include <stddef.h>

#define ETHERNET_ADDR_LEN 6
#define ETHERNET_HEADER_LEN 14

typedef struct ethernet_header
{
    uint8_t dest_mac[ETHERNET_ADDR_LEN];
    uint8_t src_mac[ETHERNET_ADDR_LEN];
    uint16_t ethertype;
} t_ethernet_header;

void print_mac(const uint8_t mac[ETHERNET_ADDR_LEN]);
void print_ethernet(const t_ethernet_header *header);
int ethernet_parse(const uint8_t *data, size_t length,
        t_ethernet_header *header);

#endif
