#include "custom_function.h"

int createSocketBC()
{
    int sd, on = 1;
    
    if ((sd = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
        exit_error("createSocketBC(): socket() error");
    }
    if (setsockopt(sd, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on)) < 0) {
        exit_error("createSocketBC(): setsockopt() failed set SO_BROADCAST");
    }
    if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0) {
        exit_error("createSocketBC(): setsockopt() failed set SO_REUSEADDR");
    }
    bindSocketOnInterface(sd);

    struct sockaddr_in sa;
    sa.sin_family = AF_INET;
    sa.sin_port = htons(67);
    sa.sin_addr.s_addr = htonl(INADDR_ANY); // Bind on your address
    // sa.sin_addr.s_addr = inet_addr("0.0.0.0"); 

    if (bind(sd, (struct sockaddr*) &sa, sizeof(sa)) < 0) {
        exit_error("createSocketBC(): bind() error");
    }
    
    return sd;
}