
#include "../include/ipv4.h"
#include <string.h>

int ipv4_parse(const uint8_t *data, size_t length,
        t_ipv4_header *header)
{
    size_t header_length;

    if (data == NULL || header == NULL)
        return -1;

    if (length < IPV4_MIN_HEADER_LEN)
        return -1;

    header->version = data[0] >> 4;
    header->ihl = data[0] & 0x0F;

    if (header->version != 4)
        return -1;

    header_length = (size_t)header->ihl * 4;

    if (header_length < IPV4_MIN_HEADER_LEN
        || header_length > length)
        return -1;

    header->total_length = ((uint16_t)data[2] << 8)
        | (uint16_t)data[3];

    if (header->total_length < header_length)
        return -1;

    header->ttl = data[8];
    header->protocol = data[9];

    memcpy(header->src_ip, data + 12, IPV4_ADDR_LEN);
    memcpy(header->dest_ip, data + 16, IPV4_ADDR_LEN);

    return 0;
}
