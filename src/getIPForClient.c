#include "custom_function.h"

uint32_t getIPForClient()
{
    uint32_t startIpPool, endIpPool, currentIp;
    startIpPool = inet_network(IP_POOL_START);
    endIpPool   = inet_network(IP_POOL_END);

    struct in_addr addr;
    char* strIp;

    for (currentIp = startIpPool; currentIp <= endIpPool; ++currentIp) {
        addr.s_addr = ntohl(currentIp);
        strIp = inet_ntoa(addr);

        if (checkBusyIp(strIp)) {
            continue;
        }

        if (!checkBusyIp(strIp) && checkAvailableHost(strIp)) {
            return addr.s_addr;
        }
    }

    return 0; // if not found free ip
}