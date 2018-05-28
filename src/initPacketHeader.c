#include "custom_function.h"

int length = 0;

struct in_addr getMyIpAddr()    
{          
	int sock;
	struct sockaddr_in sin;
	struct ifreq ifr;

	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == -1) {
			perror("socket");
			exit(1);
	}

	strncpy(ifr.ifr_name, USED_INTERFACE, 16);
	ifr.ifr_name[16 - 1] = 0;

	if (ioctl(sock, SIOCGIFADDR, &ifr) < 0)
	{
			perror("ioctl");
			exit(1);
	}

	memcpy(&sin, &ifr.ifr_addr, sizeof(sin));
	return sin.sin_addr;
}


void addopt1(struct dhcp_msg *packet) // Subnet Mask
{
	packet->option[length] = 0x01 ;
	packet->option[length+1] = 0x04;
	packet->option[length+2] = 0xff;
	packet->option[length+3] = 0xff;
	packet->option[length+4] = 0xff;
	packet->option[length+5] = 0x00;

	length += 6;
}

void addopt3(struct dhcp_msg *packet) // Router
{
    packet->option[length] = 0x03 ;
    packet->option[length+1] = 0x04;

    struct in_addr ip;
    ip = getMyIpAddr();
    memcpy(packet->option + length + 2, &ip, 4);
    
    length += 6;
}

void addopt6(struct dhcp_msg *packet) // DNS
{	
	packet->option[length] = 0x06;
	packet->option[length+1] = 0x08;
	// 8.8.8.8
	packet->option[length+2] = 0x08;
	packet->option[length+3] = 0x08;
	packet->option[length+4] = 0x08;
	packet->option[length+5] = 0x08;
	// 8.8.8.9
	packet->option[length+6] = 0x08;
	packet->option[length+7] = 0x08;
	packet->option[length+8] = 0x08;
	packet->option[length+9] = 0x09;
	
	length += 10;
}

void addopt53(struct dhcp_msg *packet, int type) // DHCP Message Type
{	
	packet->option[length] = 0x35;
	packet->option[length+1] = 0x01;
	packet->option[length+2] = type;

	length += 3;
}	

void addopt51(struct dhcp_msg *packet) // IP Address Lease Time
{	
	packet->option[length] = 0x33;
	packet->option[length+1] = 0x04;
	packet->option[length+2] = 0;
	packet->option[length+3] = 0;
	packet->option[length+4] = TOTAL_TIME_1;
	packet->option[length+5] = TOTAL_TIME_0;

	length += 6;
}

void addopt54(struct dhcp_msg *packet) // DHCP Server identifier
{	
	packet->option[length] = 0x36;
	packet->option[length+1] = 0x04;
	// 192.168.56.1
	packet->option[length+2] = 0xc0;
	packet->option[length+3] = 0xa8;
	packet->option[length+4] = 0x38;
	packet->option[length+5] = 0x01;

	length += 6;
}

// void addopt55(struct dhcp_msg *packet) // Only for client
// {	
// 	packet->option[length] = 0x37;
// 	packet->option[length+1] = 0x0c;
// 	packet->option[length+2] = 0x01;
// 	packet->option[length+3] = 0x0f;
// 	packet->option[length+4] = 0x03;
// 	packet->option[length+5] = 0x06;
// 	packet->option[length+6] = 0x2c;
// 	packet->option[length+7] = 0x2e;
// 	packet->option[length+8] = 0x2f;
// 	packet->option[length+9] = 0x1f;
// 	packet->option[length+10] = 0x21;
// 	packet->option[length+11] = 0x79;
// 	packet->option[length+12] = 0xf9;
// 	packet->option[length+13] = 0x2b;

// 	length += 14;
// }

void addopt58(struct dhcp_msg *packet) // Renewal Time Value
{	
	packet->option[length] = 0x3a;
	packet->option[length+1] = 0x04;
	packet->option[length+2] = 0;
	packet->option[length+3] = 0;
	packet->option[length+4] = TOTAL_TIME_1 / 2;
	packet->option[length+5] = TOTAL_TIME_0 / 2;

	length += 6;
}

void addopt59(struct dhcp_msg *packet) // Rebinding Time Value
{	
	packet->option[length] = 0x3b;
	packet->option[length+1] = 0x04;
	packet->option[length+2] = 0;
	packet->option[length+3] = 0;
	packet->option[length+4] = (TOTAL_TIME_1 / 8) * 7;
	packet->option[length+5] = (TOTAL_TIME_0 / 8) * 7;

	length += 6;
}

// void addopt60(struct dhcp_msg *packet) // Only for client
// {
// 	packet->option[length] = 0x3c;
// 	packet->option[length+1] = 0x05;
// 	packet->option[length+2] = 0x20;
// 	packet->option[length+3] = 0x14;
// 	packet->option[length+4] = 0x21;
// 	packet->option[length+5] = 0x32;
// 	packet->option[length+6] = 0x79;

// 	length += 7;
// }

void addopt255(struct dhcp_msg *packet) // End
{	
	packet->option[length] = 0xff;
}

void initPacketHeader(struct dhcp_msg *packet, uint8_t type)
{
	length = 0;
    memset(packet->option, 0, sizeof(packet->option));
    packet->hdr.op = BOOTREPLY;

	switch (type) {
		case DHCP_OFFER:
			packet->hdr.secs = 0x0000;
		    addopt53(packet, type);
		    addopt51(packet);
		    addopt1(packet);
		    addopt3(packet);
		    addopt6(packet);
		    addopt54(packet);
		    addopt58(packet);
		    addopt59(packet);
		    break;

		case DHCP_ACK:
			packet->hdr.secs = 0x0000;
		    addopt53(packet, type);
		    addopt51(packet);
		    addopt1(packet);
		    addopt3(packet);
		    addopt6(packet);
		    addopt54(packet);
		    addopt58(packet);
		    addopt59(packet);
		    break;

		case DHCP_IACK:
			packet->hdr.flags = 0x0000;
		    addopt53(packet, DHCP_ACK);
		    addopt1(packet);
		    addopt3(packet);
		    addopt6(packet);
		    addopt54(packet);
		    addopt58(packet);
		    addopt59(packet);
		    break;

		case DHCP_NAK:
			packet->hdr.ciaddr = 0;
			packet->hdr.yiaddr = 0;
			packet->hdr.flags = 0x0080;
			packet->hdr.secs = 0x0000;
		    packet->hdr.siaddr = 0x00000000;
		    packet->hdr.giaddr = 0;  
		    packet->hdr.dhcp_magic = 0x63538263;
		    addopt53(packet, type);
		    addopt54(packet);
			break;

        default:
        exit_error("initPacketHeader(): Unknown packet type");
	}

    addopt255(packet);
}