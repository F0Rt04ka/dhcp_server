#include "custom_function.h"

struct dhcp_msg* receivePacketDHCP(int sockDesc)
{
    struct dhcp_msg* message;
	struct sockaddr_in clientAddr;
	unsigned int cliAddrLen, recvMsgSize;
	
	message = (dhcp_msg *) malloc(sizeof(dhcp_msg));
	memset(message, 0, sizeof(dhcp_msg));

	if ((recvMsgSize = recvfrom(sockDesc, message, MAX_MSG_LEN, 0,(struct sockaddr *) &clientAddr, &cliAddrLen)) < 0) {
        exit_error("receivePacketDHCP(): recvfrom() error.");
    }
	
	return message;
}