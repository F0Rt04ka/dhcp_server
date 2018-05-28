#ifndef CUSTOM_FUNCTION_H
#define CUSTOM_FUNCTION_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <sys/ioctl.h>        // macro ioctl is defined
#include <bits/ioctls.h>      // defines values for argument "request" of ioctl.
#include <time.h>

#include "custom_types.h"
#include "config.h"

int checkAvailableHost (char* hostIP);

int createSocketBC();

struct dhcp_msg* receivePacketDHCP(int sockDesc);
void sendPacketDHCP(enum dhcp_msg_type type, int sock, struct dhcp_msg* message);

void initPacketHeader(struct dhcp_msg* packet, uint8_t type);

void bindSocketOnInterface(int sockDesc);

struct lease * getLeaseDataFromDhcpHeader(struct dhcp_header header);
int checkBusyIp(char* ip);
int writeIpLease(struct lease* iplease);
int removeIpFromLeaseList(char* ip);
int readIpLease(struct lease*** ipLeaseList, int* countIp);

uint32_t getIPForClient();

void exit_error(char* message);
void info_message(char* message);
void error_message(char* message);

char * allocate_strmem(int len);
uint8_t * allocate_ustrmem(int len);
int * allocate_intmem(int len);

#endif // !CUSTOM_FUNCTION_H