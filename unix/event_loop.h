//
// Created by Michal Ziobro on 23/05/2017.
//

#ifdef __unix__

#include <dns_sd.h>

#ifndef NETWORK_SERVICE_DISCOVERY_EVENT_LOOP_H
#define NETWORK_SERVICE_DISCOVERY_EVENT_LOOP_H

void nsd_handle_events(DNSServiceRef serviceRef);

#endif //NETWORK_SERVICE_DISCOVERY_EVENT_LOOP_H


#endif
