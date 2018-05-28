#include "custom_function.h"

int main(int argc, char const *argv[])
{
    int sockDesc = createSocketBC();
    struct dhcp_msg *message;
    struct in_addr addr;

    info_message("Start server");
    while (1) {
        message = receivePacketDHCP(sockDesc);

        switch (message->option[2]) {
            case DHCP_INFORM:
                if (message->hdr.op != 0) {
                    info_message("Received DHCP_INFORM");
                    sendPacketDHCP(DHCP_IACK, sockDesc, message);
                    printf("Send DHCP_ACK\n");
                }
            break;

            case DHCP_RELEASE:
                info_message("Received DHCP_RELEASE");
                
                addr.s_addr = message->hdr.ciaddr;
                char* ipStr = inet_ntoa(addr);
                
                if (removeIpFromLeaseList(ipStr)) {
                    printf("%s removed from lease list\n", ipStr);
                }
            break;

            case DHCP_DISCOVER:
                info_message("Received DHCP_DISCOVER");
                // TODO: check 50opt for Requested IP Address

                if ((message->hdr.yiaddr = getIPForClient())) {
                    sendPacketDHCP(DHCP_OFFER, sockDesc, message);
                } else {
                    error_message("Ip pool error. All available ip is used");
                }
            break;

            case DHCP_REQUEST:
                info_message("Received DHCP_REQUEST");

                if (message->hdr.ciaddr != 0) {
                    //check client ip. If client ip === sent ip send ACK on this ip addr
                    addr.s_addr = message->hdr.yiaddr = message->hdr.ciaddr;
                    printf("Renew IP: %s\n", inet_ntoa(addr));
                    removeIpFromLeaseList(inet_ntoa(addr));
                } else {
                    addr.s_addr = message->hdr.yiaddr = getIPForClient();
                    printf("Allocated IP: %s\n", inet_ntoa(addr));
                }

                writeIpLease(getLeaseDataFromDhcpHeader(message->hdr));
                sendPacketDHCP(DHCP_ACK, sockDesc, message);
            break;
        }

        free(message);
    }

    return 0;
}