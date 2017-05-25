//
// Created by Michal Ziobro on 23/05/2017.
//



#ifdef __APPLE__

#ifndef NETWORK_SERVICE_DISCOVERY_EVENT_LOOP_H_H
#define NETWORK_SERVICE_DISCOVERY_EVENT_LOOP_H_H

#include <dns_sd.h>

void nsd_handle_events(DNSServiceRef serviceRef);

#endif //NETWORK_SERVICE_DISCOVERY_EVENT_LOOP_H_H

#endif // __APPLE__