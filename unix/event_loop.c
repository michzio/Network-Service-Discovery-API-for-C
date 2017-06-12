//
// Created by Michal Ziobro on 23/05/2017.
//

#ifdef __unix__

#include "event_loop.h"
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

#define LONG_TIME 100000000
static volatile int time_out = LONG_TIME;

void nsd_handle_events(nsd_context_t *nsd_context) {

    DNSServiceRef serviceRef = (DNSServiceRef) nsd_context->context;
    nsd_state_t *nsd_state = malloc(sizeof(nsd_state_t));
    nsd_state->serviceRef = serviceRef;
    nsd_context->context = nsd_state;

    // get file descriptor of DNS service for the Unix Domain Socket
    // used to communicate with the mdnsd deamon running in the background
    int dns_sd_fd = DNSServiceRefSockFD(serviceRef);
    int nfds = dns_sd_fd + 1;
    fd_set read_fds;
    struct timeval tv;

    nsd_state->dns_sd_fd = dns_sd_fd;

    while(1) {

        FD_ZERO(&read_fds);
        FD_SET(dns_sd_fd, &read_fds);

        tv.tv_sec = time_out;
        tv.tv_usec = 0;
        int result = select(nfds, &read_fds, (fd_set *) NULL, (fd_set *) NULL, &tv);
        if( result > 0) {

            DNSServiceErrorType  err = kDNSServiceErr_NoError;

            if(FD_ISSET(dns_sd_fd, &read_fds)) {
                // DNSServiceProcessResult() should be called when data are ready to read
                // otherwise it will block and wait until there are some data
                err = DNSServiceProcessResult(serviceRef);
                if(err) break;
            }
        } else {

            fprintf(stderr, "select() returned %d errno %d %s \n", result, errno, strerror(errno));
            if(errno != EINTR) break;
        }
    }

    // remove dns_sd_fd file descriptor from FD_SET
    FD_CLR(dns_sd_fd, &read_fds);
}

#endif // __unix__