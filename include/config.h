#ifndef CONFIG_H
#define CONFIG_H

// Time max val 255 in 0x00 format (it`s 10 minutes 0x0258)
#define TOTAL_TIME_1 0x00
#define TOTAL_TIME_0 0x28


#define USED_INTERFACE "vboxnet0"
#define MAX_MSG_LEN 512

#define IP_POOL_START "192.168.56.10"
#define IP_POOL_END "192.168.56.20"

#define LEASE_FILENAME "dhcp.lease"

#endif // !CONFIG_H