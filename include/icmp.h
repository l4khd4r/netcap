
#ifndef ICMP_H
#define ICMP_H

#include <stdint.h>
#include <stddef.h>

#define ICMP_MIN_HEADER_LEN 4
#define ICMP_ECHO_HEADER_LEN 8

typedef struct icmp_header
{
    uint8_t type;
    uint8_t code;
    uint16_t checksum;
    uint16_t identifier;
    uint16_t sequence_number;
    uint8_t has_echo_fields;
} t_icmp_header;

int icmp_parse(const uint8_t *data, size_t length,
        t_icmp_header *header);

void print_icmp(const t_icmp_header *header);

#endif
