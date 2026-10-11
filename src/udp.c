#include "../include/udp.h"

#include <stdio.h>
static uint16_t read_be16(const uint8_t *data)
{
    return ((uint16_t)data[0] << 8) | (uint16_t)data[1];
}


int udp_parse(const uint8_t *data , size_t length , t_udp_header *header)
{
    if(data == NULL || header == NULL)
        return -1;


    if(length < UDP_MIN_HEADER_LEN)
        return -1;


    header->src_port = read_be16(data);
    header->dest_port = read_be16(data + 2);
    header->length = read_be16(data + 4);
    header->checksum = read_be16(data + 6);

    if(header->length < UDP_MIN_HEADER_LEN )
        return -1;

    if(header->length > length)
        return -1;

    return 0;
}

void print_udp(const t_udp_header *header)
{
    printf("UDP\n");
    printf("  Source port:      %u\n", header->src_port);
    printf("  Destination port: %u\n", header->dest_port);
    printf("  Length:           %u bytes\n", header->length);
    printf("  Checksum:         0x%04X\n", header->checksum);
}