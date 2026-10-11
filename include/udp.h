#ifndef UDP_H

#define UDP_H

#include <stdint.h>
#include <stddef.h>

#define UDP_MIN_HEADER_LEN 8
typedef struct s_udp_header {
    uint16_t src_port ;
    uint16_t dest_port ;
    uint16_t length ;
    uint16_t checksum ;
}t_udp_header;


int udp_parse( const uint8_t *data , size_t length , t_udp_header *header);
void print_udp(const t_udp_header *header);
#endif