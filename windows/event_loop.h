//
// Created by Michal Ziobro on 24/05/2017.
//

#ifdef _WIN32

#ifndef NETWORK_SERVICE_DISCOVERY_EVENT_LOOP_H
#define NETWORK_SERVICE_DISCOVERY_EVENT_LOOP_H

#include "../../../../../../../../Program Files/Bonjour SDK/Include/dns_sd.h"
#include "../network_service_discovery.h"

// window messages related state
struct nsd_state {
    DNSServiceRef serviceRef;
};
typedef struct nsd_state nsd_state_t;

void nsd_handle_events(nsd_context_t *nsd_context);

#endif //NETWORK_SERVICE_DISCOVERY_EVENT_LOOP_H

#endif
