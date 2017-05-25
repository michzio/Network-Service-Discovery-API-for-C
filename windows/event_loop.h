//
// Created by Michal Ziobro on 24/05/2017.
//

#ifdef _WIN32

#ifndef NETWORK_SERVICE_DISCOVERY_EVENT_LOOP_H
#define NETWORK_SERVICE_DISCOVERY_EVENT_LOOP_H

#include "../../../../../../../../Program Files/Bonjour SDK/Include/dns_sd.h"

void nsd_handle_events(DNSServiceRef serviceRef);

#endif //NETWORK_SERVICE_DISCOVERY_EVENT_LOOP_H

#endif
