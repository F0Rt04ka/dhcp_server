#include "custom_function.h"

#define IP4_HDRLEN 20
#define ICMP_HDRLEN 8

// Computing the internet checksum (RFC 1071).
// Note that the internet checksum does not preclude collisions.
uint16_t checksum(uint16_t *addr, int len);

int checkAvailableHost(char* hostIP) // ping function
{   
    int sd, *ip_flags, datalen = 4;
    const int on = 1;
    uint8_t *packet, *icmpData;
    struct sockaddr_in dst_addr;
    struct ip iphdr;
    struct icmp icmphdr;

    icmpData = allocate_ustrmem(IP_MAXPACKET);
    packet = allocate_ustrmem(IP_MAXPACKET);
    ip_flags = allocate_intmem(datalen);

    icmpData[0] = 'T';
    icmpData[1] = 'e';
    icmpData[2] = 's';
    icmpData[3] = 't';

    iphdr.ip_hl = IP4_HDRLEN / sizeof (uint32_t);
    iphdr.ip_v = 4;
    iphdr.ip_tos = 0;
    iphdr.ip_len = htons(IP4_HDRLEN + ICMP_HDRLEN + datalen);
    iphdr.ip_id = htons(0);

    ip_flags[0] = 0; // Zero (1 bit)
    ip_flags[1] = 0; // Do not fragment flag (1 bit)
    ip_flags[2] = 0; // More fragments following flag (1 bit)
    ip_flags[3] = 0; // Fragmentation offset (13 bits)

    iphdr.ip_off = htons ((ip_flags[0] << 15)
                      + (ip_flags[1] << 14)
                      + (ip_flags[2] << 13)
                      +  ip_flags[3]);

    iphdr.ip_ttl = 255;
    iphdr.ip_p = IPPROTO_ICMP;
    iphdr.ip_src.s_addr = INADDR_ANY;

    if (inet_pton(AF_INET, hostIP, &iphdr.ip_dst) != 1) {
        exit_error("checkAvailableHost(): inet_pton() error");
    }

    iphdr.ip_sum = 0;
    iphdr.ip_sum = checksum((uint16_t *) &iphdr, IP4_HDRLEN);

    icmphdr.icmp_type = ICMP_ECHO;
    icmphdr.icmp_code = 0;
    icmphdr.icmp_id = htons(1000); // Identifier (16 bits): usually pid of sending process - pick a number
    icmphdr.icmp_seq = htons(0);
    icmphdr.icmp_cksum = 0;

    memcpy (packet, &iphdr, IP4_HDRLEN); // First part is an IPv4 header.
    memcpy ((packet + IP4_HDRLEN), &icmphdr, ICMP_HDRLEN); // Next part of packet is upper layer protocol header.
    memcpy (packet + IP4_HDRLEN + ICMP_HDRLEN, icmpData, datalen); // Finally, add the ICMP data.
    // Calculate ICMP header checksum
    icmphdr.icmp_cksum = checksum((uint16_t *) (packet + IP4_HDRLEN), ICMP_HDRLEN + datalen);
    memcpy ((packet + IP4_HDRLEN), &icmphdr, ICMP_HDRLEN);

    memset (&dst_addr, 0, sizeof (struct sockaddr_in));
    dst_addr.sin_family = AF_INET;
    dst_addr.sin_addr.s_addr = iphdr.ip_dst.s_addr;

    // Submit request for a raw socket descriptor.
    if ((sd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0) {
        exit_error("checkAvailableHost(): socket() failed");
    }
    // Set flag so socket expects us to provide IPv4 header.
    if (setsockopt (sd, IPPROTO_IP, IP_HDRINCL, &on, sizeof (on)) < 0) {
        exit_error("checkAvailableHost(): setsockopt() failed to set IP_HDRINCL");
    }

    struct timeval tv = {2, 0};
    if (setsockopt (sd, SOL_SOCKET, SO_RCVTIMEO, (char *) &tv, sizeof (tv)) < 0) {
        exit_error("checkAvailableHost(): setsockopt() failed to set SO_RCVTIMEO");
    }

    bindSocketOnInterface(sd);

    if (sendto(sd, packet, IP4_HDRLEN + ICMP_HDRLEN + datalen, 0, (struct sockaddr *)&dst_addr, sizeof (struct sockaddr)) < 0)  {
        exit_error("checkAvailableHost(): sendto() failed");
    }

    char *msg;
    msg = allocate_strmem(100);
    recv(sd, msg, 100, 0);

    int result, len = strlen(msg);
    if ((len == 0) || (len % 2 == 0)) {
        result = 1;
    } else {
        result = 0;
    }

    close(sd);
    free(packet);
    free(ip_flags);
    free(icmpData);
    free(msg);

    return result;
}

// Computing the internet checksum (RFC 1071).
// Note that the internet checksum does not preclude collisions.
uint16_t checksum(uint16_t *addr, int len)
{
    int count = len;
    uint32_t sum = 0;
    uint16_t answer = 0;

    // Sum up 2-byte values until none or only one byte left.
    while (count > 1) {
        sum += *(addr++);
        count -= 2;
    }

    // Add left-over byte, if any.
    if (count > 0) {
        sum += *(uint8_t *) addr;
    }

    // Fold 32-bit sum into 16 bits; we lose information by doing this,
    // increasing the chances of a collision.
    // sum = (lower 16 bits) + (upper 16 bits shifted right 16 bits)
    while (sum >> 16) {
        sum = (sum & 0xffff) + (sum >> 16);
    }

    // Checksum is one's compliment of sum.
    answer = ~sum;

    return (answer);
}