#include "custom_function.h"

// Create lease structure from dhcp header
struct lease * getLeaseDataFromDhcpHeader(struct dhcp_header header)
{
    struct in_addr addr;
    struct lease* ipLease;
    
    ipLease = (struct lease*) malloc(sizeof(struct lease));
    memset(ipLease, 0, sizeof(struct lease));

    if (header.ciaddr != 0) {
        addr.s_addr = header.ciaddr;
    } else if (header.yiaddr != 0) {
        addr.s_addr = header.yiaddr;
    }

    strcpy(ipLease->ipaddr, inet_ntoa(addr));
    sprintf(ipLease->macaddr, "%.2x:%.2x:%.2x:%.2x:%.2x:%.2x", 
        header.chaddr[0], 
        header.chaddr[1], 
        header.chaddr[2], 
        header.chaddr[3], 
        header.chaddr[4], 
        header.chaddr[5]
    );

    return ipLease;
}