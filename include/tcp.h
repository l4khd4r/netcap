#ifndef TCP_H
#define TCP_H


#include <stdint.h>
#include <stddef.h>

#define TCP_MIN_HEADER_LEN 20


typedef struct s_tcp_header {
    uint16_t src_port ;
    uint16_t dest_port ;
    uint32_t sequence_number ;
    uint32_t acknowledgment_number;
    uint8_t data_offset ;
    uint16_t flags;
    uint16_t window_size;

} t_tcp_header;

int tcp_parse(const uint8_t *data , size_t length , t_tcp_header *header);
void print_tcp(const t_tcp_header *header);
#endif