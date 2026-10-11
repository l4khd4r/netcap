#include "../include/tcp.h"



#include <stdio.h>


static uint16_t read_be16(const uint8_t *data)
{
    return ((uint16_t)data[0] << 8) | (uint16_t)data[1];
}


static uint32_t read_be32(const uint8_t *data)
{
    return ((uint32_t)data[0] << 24) | ((uint32_t)data[1] << 16)
        | ((uint32_t)data[2] << 8) | (uint32_t)data[3];
}

void print_tcp(const t_tcp_header *header)
{
    printf("TCP\n");
    printf("  Source port:      %u\n", header->src_port);
    printf("  Destination port: %u\n", header->dest_port);
    printf("  Sequence number:  %u\n", header->sequence_number);
    printf("  Acknowledgment:   %u\n", header->acknowledgment_number);
    printf("  Data offset:      %u bytes\n", header->data_offset * 4);
    printf("  Flags:            0x%03X\n", header->flags);
    printf("  Window size:      %u\n", header->window_size);
}

int tcp_parse(const uint8_t *data, size_t length , t_tcp_header *header)
{
    size_t header_length ;
    if(data == NULL || header == NULL)
        return -1;

    if(length < TCP_MIN_HEADER_LEN)
        return -1;

    header_length = (size_t)(data[12] >> 4) * 4;

    if(header_length < TCP_MIN_HEADER_LEN || header_length > length)
        return -1;
    header->src_port = read_be16(data);
    header->dest_port = read_be16(data + 2);
    header->sequence_number = read_be32(data + 4);
    header->acknowledgment_number = read_be32(data + 8);
    header->data_offset = (uint8_t)(data[12] >> 4);
    /*
    * NS is bit 8; the remaining eight flags
    * are stored in byte 13
    */
    header->flags = ((uint16_t)(data[12] & 0x01) << 8) | (uint16_t)data[13];

    return 0;
}


