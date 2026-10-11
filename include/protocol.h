#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "ipv4.h"
#include "ethernet.h"
#include <stdint.h>


const char *ipv4_protocol_name(uint8_t protocol);

#endif