#ifndef CUSTOM_TYPES_H
#define CUSTOM_TYPES_H

#include "custom_function.h"

struct lease
{
	char ipaddr[30];
	char macaddr[30];
	char ttime[30];
};

struct dhcp_header {
    uint8_t op;      // message op code, message type
    uint8_t htype;   // hardware address type
    uint8_t hlen;    // hardware address length
    uint8_t hops;    // incremented by relay agents

    uint32_t xid;    // transaction ID

    uint16_t secs;   // seconds since address acquisition or renewal
    uint16_t flags;  // flags

    uint32_t ciaddr; // client IP address
    uint32_t yiaddr; // 'your' client IP address
    uint32_t siaddr; // IP address of the next server to use in bootstrap
    uint32_t giaddr; // relay agent IP address

    uint8_t chaddr[16]; // client hardware address

    uint8_t sname[64]; // server host name

    uint8_t file[128]; // boot file name

  	uint32_t dhcp_magic;
};

typedef struct dhcp_header dhcp_header;

struct dhcp_msg {
    dhcp_header hdr;

    //dhcp_option_list list[10];
    uint8_t option[256];
   
};

typedef struct dhcp_msg dhcp_msg;

enum dhcp_msg_type {
     DHCP_DISCOVER = 1,
     DHCP_OFFER    = 2,
     DHCP_REQUEST  = 3,
     DHCP_DECLINE  = 4,
     DHCP_ACK      = 5,
     DHCP_NAK      = 6,
     DHCP_RELEASE  = 7,
     DHCP_INFORM   = 8,
     DHCP_IACK     = 9, // fake type. Only for response to DHCP_INFORM
};

enum op_types {
    BOOTREQUEST = 1,
    BOOTREPLY   = 2,   
};

enum hardware_address_types {
    ETHERNET     = 0x01,
    ETHERNET_LEN = 0x06,
};

#endif // !CUSTOM_TYPES_H