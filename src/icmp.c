
#include "../include/icmp.h"
#include <stdio.h>

static uint16_t read_be16(const uint8_t *data)
{
    return ((uint16_t)data[0] << 8)
        | (uint16_t)data[1];
}

int icmp_parse(const uint8_t *data, size_t length,
        t_icmp_header *header)
{
    if (data == NULL || header == NULL)
        return -1;

    if (length < ICMP_MIN_HEADER_LEN)
        return -1;

    header->type = data[0];
    header->code = data[1];
    header->checksum = read_be16(data + 2);
    header->identifier = 0;
    header->sequence_number = 0;
    header->has_echo_fields = 0;

    if (header->type == 0 || header->type == 8)
    {
        if (length < ICMP_ECHO_HEADER_LEN)
            return -1;

        header->identifier = read_be16(data + 4);
        header->sequence_number = read_be16(data + 6);
        header->has_echo_fields = 1;
    }

    return 0;
}

void print_icmp(const t_icmp_header *header)
{
    printf("\033[36mICMP\033[0m\n");
    printf("  Type:     %u\n", header->type);
    printf("  Code:     %u\n", header->code);
    printf("  Checksum: 0x%04X\n", header->checksum);

    if (header->type == 8 && header->code == 0)
        printf("  Message:  Echo Request\n");
    else if (header->type == 0 && header->code == 0)
        printf("  Message:  Echo Reply\n");
    else if (header->type == 3)
        printf("  Message:  Destination Unreachable\n");
    else if (header->type == 11)
        printf("  Message:  Time Exceeded\n");
    else
        printf("  Message:  Other ICMP message\n");

    if (header->has_echo_fields)
    {
        printf("  Identifier:      %u\n", header->identifier);
        printf("  Sequence number: %u\n", header->sequence_number);
    }
}
