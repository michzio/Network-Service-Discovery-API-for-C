//
// Created by Michal Ziobro on 22/05/2017.
//

#ifndef NETWORKSERVICEDISCOVERYAPI_NETWORK_SERVICE_DISCOVERY_H
#define NETWORKSERVICEDISCOVERYAPI_NETWORK_SERVICE_DISCOVERY_H

#ifdef __APPLE__
#include <dns_sd.h>
#endif // __APPLE__

#ifdef _WIN32
#include "../../../../../../../Program Files/Bonjour SDK/Include/dns_sd.h"
#endif // _WIN32

#ifdef __unix__
#include <dns_sd.h>
#endif // __unix__


typedef enum {ADDED = 1, REMOVED = 2, MORE = 4} nsd_flags_t;
typedef enum { REGISTER_CALLBACK, BROWSE_CALLBACK, RESOLVE_CALLBACK } callback_type_t;

typedef void (*nsd_register_callback_t)(const char *name, const char *regtype, const char *domain, nsd_flags_t flags, void *context);
typedef void (*nsd_browse_callback_t)(uint32_t interface_idx, const char *service_name, const char *regtype, const char *domain, nsd_flags_t flags, void *context);
typedef void (*nsd_resolve_callback_t)(uint32_t interface_idx, const char *fullname, const char *hosttarget, uint16_t port, nsd_flags_t flags, void *context);

struct nsd_context {
    void *context;
    callback_type_t callback_type;
    union {
        nsd_register_callback_t register_callback;
        nsd_browse_callback_t browse_callback;
        nsd_resolve_callback_t resolve_callback;
    } callback;
};
typedef struct nsd_context nsd_context_t;

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
