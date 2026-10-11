
#include "../include/ethernet.h"
#include <stdio.h>
#include <string.h>

void print_mac(const uint8_t mac[ETHERNET_ADDR_LEN])
{
    printf("%02X:%02X:%02X:%02X:%02X:%02X",
        mac[0], mac[1], mac[2],
        mac[3], mac[4], mac[5]);
}

void print_ethernet(const t_ethernet_header *header)
{
    printf("Ethernet II\n");
    printf("  Destination: ");
    print_mac(header->dest_mac);
    printf("\n  Source:      ");
    print_mac(header->src_mac);
    printf("\n  EtherType:   0x%04X\n", header->ethertype);
}

int ethernet_parse(const uint8_t *data, size_t length,
        t_ethernet_header *header)
{
    if (data == NULL || header == NULL)
        return -1;

    if (length < ETHERNET_HEADER_LEN)
        return -1;

    memcpy(header->dest_mac, data, ETHERNET_ADDR_LEN);
    memcpy(header->src_mac, data + ETHERNET_ADDR_LEN,
        ETHERNET_ADDR_LEN);

    header->ethertype = ((uint16_t)data[12] << 8)
        | (uint16_t)data[13];

    return 0;
}
