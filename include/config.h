#ifndef CONFIG_H
#define CONFIG_H

    // Time max val 255 in 0x00 format (10min (600sec) == 0x0258)
    #define TOTAL_TIME_1 0x00
    #define TOTAL_TIME_0 0x28

    #define USED_INTERFACE "vboxnet0"

    #define IP_POOL_START "192.168.56.10"
    #define IP_POOL_END "192.168.56.20"

    #define LEASE_FILENAME "dhcp.lease"


    #define MAX_MSG_LEN 512

#endif // !CONFIG_H