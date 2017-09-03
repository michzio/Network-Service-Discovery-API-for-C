//
// Created by Michal Ziobro on 23/05/2017.
//



#ifdef __APPLE__

#ifndef NETWORK_SERVICE_DISCOVERY_EVENT_LOOP_H_H
#define NETWORK_SERVICE_DISCOVERY_EVENT_LOOP_H_H

#include <dns_sd.h>
#include <CoreFoundation/CoreFoundation.h>
#include "../include/network_service_discovery.h"

// CFRunLoop-related state
struct nsd_state {
    DNSServiceRef serviceRef;
    CFSocketRef cfSocketRef;
    CFRunLoopSourceRef sourceRef;
    CFRunLoopRef runLoopRef;
};
typedef struct nsd_state nsd_state_t;

void nsd_handle_events(nsd_context_t *nsd_context);
void nsd_add_event_source(CFRunLoopRef runLoopRef, nsd_state_t *nsd_state);
void nsd_remove_event_source(CFRunLoopRef runLoopRef, nsd_state_t *nsd_state);

#endif //NETWORK_SERVICE_DISCOVERY_EVENT_LOOP_H_H

#endif // __APPLE__