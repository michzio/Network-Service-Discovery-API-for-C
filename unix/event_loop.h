//
// Created by Michal Ziobro on 23/05/2017.
//

#ifdef __unix__

#ifndef NETWORK_SERVICE_DISCOVERY_EVENT_LOOP_H
#define NETWORK_SERVICE_DISCOVERY_EVENT_LOOP_H

#include <dns_sd.h>
#include "../network_service_discovery.h"

// select loop related state
struct nsd_state {
    DNSServiceRef serviceRef;
    int dns_sd_fd;
};
typedef struct nsd_state nsd_state_t;

void nsd_handle_events(nsd_context_t *nsd_context);

#endif //NETWORK_SERVICE_DISCOVERY_EVENT_LOOP_H


#endif
