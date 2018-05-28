#include "custom_function.h"

void bindSocketOnInterface(int sockDesc)
{
    struct ifreq usedIfr;
	strcpy(usedIfr.ifr_name, USED_INTERFACE);

    if (setsockopt(sockDesc, SOL_SOCKET, SO_BINDTODEVICE, (char*)&usedIfr, sizeof (usedIfr)) < 0) {
        exit_error("bindSocketOnInterface(): setsockopt() failed to bind to interface");
    }
}