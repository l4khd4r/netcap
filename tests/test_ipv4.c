#include "../include/ipv4.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

static void build_ipv4_packet(uint8_t *packet, size_t length)
{
    memset(packet, 0, length);

    packet[0] = 0x45;       /* IPv4, IHL = 5 (20 bytes) */
    packet[2] = 0x00;       /* Total length = 20 */
    packet[3] = 0x14;
    packet[8] = 64;         /* TTL */
    packet[9] = 6;          /* TCP */

    packet[12] = 192;
    packet[13] = 168;
    packet[14] = 1;
    packet[15] = 10;

    packet[16] = 8;
    packet[17] = 8;
    packet[18] = 8;
    packet[19] = 8;
}

static void test_valid_header(void)
{
    uint8_t packet[20];
    t_ipv4_header header;

    build_ipv4_packet(packet, sizeof(packet));

    assert(ipv4_parse(packet, sizeof(packet), &header) == 0);
    assert(header.version == 4);
    assert(header.ihl == 5);
    assert(header.total_length == 20);
    assert(header.ttl == 64);
    assert(header.protocol == 6);

    assert(header.src_ip[0] == 192);
    assert(header.src_ip[1] == 168);
    assert(header.src_ip[2] == 1);
    assert(header.src_ip[3] == 10);

    assert(header.dest_ip[0] == 8);
    assert(header.dest_ip[1] == 8);
    assert(header.dest_ip[2] == 8);
    assert(header.dest_ip[3] == 8);
}

static void test_invalid_version(void)
{
    uint8_t packet[20];
    t_ipv4_header header;

    build_ipv4_packet(packet, sizeof(packet));
    packet[0] = 0x65; /* Version = 6, IHL = 5 */

    assert(ipv4_parse(packet, sizeof(packet), &header) == -1);
}

static void test_invalid_ihl(void)
{
    uint8_t packet[20];
    t_ipv4_header header;

    build_ipv4_packet(packet, sizeof(packet));
    packet[0] = 0x44; /* IHL = 4 (16 bytes, invalid) */

    assert(ipv4_parse(packet, sizeof(packet), &header) == -1);
}

static void test_truncated_header(void)
{
    uint8_t packet[19];
    t_ipv4_header header;

    build_ipv4_packet(packet, sizeof(packet));

    assert(ipv4_parse(packet, sizeof(packet), &header) == -1);
}

static void test_truncated_options(void)
{
    uint8_t packet[20];
    t_ipv4_header header;

    build_ipv4_packet(packet, sizeof(packet));
    packet[0] = 0x46; /* IHL = 6 (24-byte header) */

    assert(ipv4_parse(packet, sizeof(packet), &header) == -1);
}

static void test_total_length_smaller_than_header(void)
{
    uint8_t packet[20];
    t_ipv4_header header;

    build_ipv4_packet(packet, sizeof(packet));
    packet[2] = 0;
    packet[3] = 19;

    assert(ipv4_parse(packet, sizeof(packet), &header) == -1);
}

static void test_total_length_exceeds_available_data(void)
{
    uint8_t packet[20];
    t_ipv4_header header;

    build_ipv4_packet(packet, sizeof(packet));
    packet[2] = 0;
    packet[3] = 40;

    assert(ipv4_parse(packet, sizeof(packet), &header) == -1);
}

static void test_fragment_fields(void)
{
    uint8_t packet[20];
    t_ipv4_header header;

    build_ipv4_packet(packet, sizeof(packet));

    /*
     * Flags = 001 (MF = 1)
     * Fragment offset = 0x0123
     */
    packet[6] = 0x21;
    packet[7] = 0x23;

    assert(ipv4_parse(packet, sizeof(packet), &header) == 0);
    assert(header.flags == 1);
    assert(header.fragment_offset == 0x0123);
}

static void test_null_arguments(void)
{
    uint8_t packet[20];
    t_ipv4_header header;

    build_ipv4_packet(packet, sizeof(packet));

    assert(ipv4_parse(NULL, sizeof(packet), &header) == -1);
    assert(ipv4_parse(packet, sizeof(packet), NULL) == -1);
}

int main(void)
{
    test_valid_header();
    test_invalid_version();
    test_invalid_ihl();
    test_truncated_header();
    test_truncated_options();
    test_total_length_smaller_than_header();
    test_total_length_exceeds_available_data();
    test_fragment_fields();
    test_null_arguments();

    printf("All IPv4 parser tests passed.\n");
    return 0;
}
