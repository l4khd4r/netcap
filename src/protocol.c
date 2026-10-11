#include "../include/protocol.h"



const char *ipv4_protocol_name(uint8_t protocol)
{
    switch (protocol) {
        case 1:
            return "ICMP";
        case 6:
            return "TCP";
        case 17:
            return "UDP";
        case 41:
            return "IPv6";
        case 47:
            return "GRE";
        case 50:
            return "ESP";
        case 89:
            return "OSPF";
        default:
            return "Unknown";
    }
}