
#include "custom_function.h"

void sendPacketDHCP(enum dhcp_msg_type type, int sock, struct dhcp_msg* message)
{
	struct sockaddr_in serverAddr;
	
	initPacketHeader(message, type);
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	
	if (type == DHCP_INFORM) {
		serverAddr.sin_addr.s_addr = message->hdr.yiaddr;
	} else if (type == DHCP_REQUEST && message->hdr.ciaddr != 0) {
		serverAddr.sin_addr.s_addr = message->hdr.ciaddr;
	} else {
	 	serverAddr.sin_addr.s_addr = inet_addr("255.255.255.255");
	}

	serverAddr.sin_port = htons(68);
	
	if (sendto(sock,message, sizeof(*message), 0, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0) {
		error_message("Send failed\n");
	}
}