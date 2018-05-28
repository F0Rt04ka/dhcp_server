#include "custom_function.h"

void bindSocketOnInterface(int sockDesc)
{
    struct ifreq usedIfr;
	strcpy(usedIfr.ifr_name, USED_INTERFACE);

    if (setsockopt(sockDesc, SOL_SOCKET, SO_BINDTODEVICE, (char*)&usedIfr, sizeof (usedIfr)) < 0) {
        exit_error("bindSocketOnInterface(): setsockopt() failed to bind to interface");
    }
}

// struct ifreq * configureInterface()
// {
    // if (usedIfr != NULL) {
    //     return usedIfr;
    // }

    // int sd;

    // if ((sd = socket (AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0) {
    //     exit_error("configureInterface(): socket() failed to get socket descriptor for using ioctl() ");
    // }

    // usedIfr = (struct ifreq*) malloc(sizeof(struct ifreq));
    // memset(usedIfr, 0, sizeof (struct ifreq));
    // snprintf (usedIfr->ifr_name, sizeof (usedIfr->ifr_name), "%s", USED_INTERFACE);

    // if (ioctl (sd, SIOCGIFINDEX, usedIfr) < 0) {
    //     exit_error("configureInterface(): ioctl() failed to find interface ");
    // }

    // close(sd);
    
    // return usedIfr;
// }