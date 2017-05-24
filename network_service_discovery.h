//
// Created by Michal Ziobro on 22/05/2017.
//

#include "dns_sd.h"

#ifndef NETWORKSERVICEDISCOVERYAPI_NETWORK_SERVICE_DISCOVERY_H
#define NETWORKSERVICEDISCOVERYAPI_NETWORK_SERVICE_DISCOVERY_H

typedef enum {ADDED = 1, REMOVED = 2, MORE = 4} nsd_flags_t;
typedef void (*nsd_register_callback_t)(const char *name, const char *regtype, const char *domain, nsd_flags_t flags);
typedef void (*nsd_browse_callback_t)(uint32_t interface_idx, const char *service_name, const char *regtype, const char *domain, nsd_flags_t flags);
typedef void (*nsd_resolve_callback_t)(uint32_t interface_idx, const char *fullname, const char *hosttarget, uint16_t port, nsd_flags_t flags);

void nsd_simple_register(const char *regtype, uint16_t port, nsd_register_callback_t callback);
int nsd_spawn_simple_register(const char *regtype, uint16_t port, nsd_register_callback_t callback);
void nsd_register(const char *name, const char *regtype, const char *domain,
                  const char *host, uint16_t port, nsd_register_callback_t callback);
int nsd_spawn_register(const char *name, const char *regtype, const char *domain,
                  const char *host, uint16_t port, nsd_register_callback_t callback);
void nsd_browse(const char *regtype, const char *domain, nsd_browse_callback_t callback);
int nsd_spawn_browse(const char *regtype, const char *domain, nsd_browse_callback_t callback);
void nsd_resolve(const char *name, const char *regtype, const char *domain,
                 const uint32_t interface_idx, nsd_resolve_callback_t callback);
int nsd_spawn_resolve(const char *name, const char *regtype, const char *domain,
                 const uint32_t interface_idx, nsd_resolve_callback_t callback);
void nsd_free(DNSServiceRef serviceRef);

#endif //NETWORKSERVICEDISCOVERYAPI_NETWORK_SERVICE_DISCOVERY_H
