//
// Created by Michal Ziobro on 22/05/2017.
//

#include "../include/network_service_discovery.h"

#ifdef __APPLE__
    #include "../apple/event_loop.h"
#endif

#ifdef __unix__
    #include "unix/event_loop.h"
#endif

#ifdef _WIN32
    #include "windows/event_loop.h"
#endif

#include <errno.h>
#include <stdio.h>
#include <string.h>

#ifndef _WIN32
    #include <pthread.h>
#include <stdlib.h>

#endif

static void nsd_register_main_callback(DNSServiceRef serviceRef,
                                       DNSServiceFlags flags,
                                       DNSServiceErrorType errorCode,
                                       const char *name,
                                       const char *regtype,
                                       const char *domain,
                                       void *context) {

    if(errorCode == kDNSServiceErr_NoError) {
        printf("nsd service registered!\n");

        if(context) {
            nsd_context_t *nsd_context = ((nsd_context_t *) context);
            nsd_context->callback.register_callback(name, regtype, domain, 0, nsd_context->context);
        }

    } else {
        fprintf(stderr, "nsd_register_main_callback: failed! error: %d\n", errorCode);
    }
}

void nsd_simple_register(const char *regtype, uint16_t port, nsd_register_callback_t callback) {
    nsd_register("", regtype, "", NULL, port, callback);
}

struct nsd_register_attrs {
    const char *name;
    const char *regtype;
    const char *domain;
    const char *host;
    uint16_t port;
    nsd_register_callback_t callback;
};
typedef struct nsd_register_attrs nsd_register_attrs_t;

void *_nsd_simple_register(void *register_attrs) {

    nsd_register_attrs_t *ra = (nsd_register_attrs_t *)register_attrs;
    nsd_simple_register(ra->regtype, ra->port, ra->callback);

    return 0;
}

#ifndef _WIN32
int nsd_spawn_simple_register(const char *regtype, uint16_t port, nsd_register_callback_t callback) {

    nsd_register_attrs_t register_attrs = {
            .regtype = regtype,
            .port = port,
            .callback = callback
    };

    pthread_t thread;

    /* create a second thread which executes nsd_simple_register() */
    if(pthread_create(&thread, NULL, _nsd_simple_register, &register_attrs)) {
        fprintf(stderr, "pthread_create: failed!\n");
        return -1;
    }

    if(pthread_detach(thread)) {
        fprintf(stderr, "pthread_detach: failed!\n");
        return -1;
    }
    return 0;
}
#endif

void nsd_register(const char *name, const char *regtype, const char *domain,
                  const char *host, uint16_t port, nsd_register_callback_t callback) {

    DNSServiceRef serviceRef;

    nsd_context_t *nsd_context = malloc(sizeof(nsd_context_t));
    nsd_context->callback_type = REGISTER_CALLBACK;
    nsd_context->callback.register_callback = callback;

    if(DNSServiceRegister(&serviceRef,
                          0, // flags - optional settings
                          0, // interfaceIndex, 0 any interface or kDNSServiceInterfaceIndexLocalOnly
                          name, // name of service, if NULL or empty string then system-wide default name is used
                          regtype,  // service type like "_remotely_click._tcp"
                          domain, // ex. "local", or can be NULL that defaults to local
                          host, // if NULL or empty string then it use current host
                          port,
                          0, // txtLen
                          NULL, // txtRecord (NULL)
                          nsd_register_main_callback,
                          &nsd_context
                         ) == kDNSServiceErr_NoError) {

        nsd_context->context = serviceRef;
        nsd_handle_events(nsd_context);
        nsd_free(serviceRef);
    }
}

void *_nsd_register(void *register_attrs) {

    nsd_register_attrs_t *ra = (nsd_register_attrs_t *)register_attrs;
    nsd_register(ra->name, ra->regtype, ra->domain, ra->host, ra->port, ra->callback);

    return 0;
}

#ifndef _WIN32
int nsd_spawn_register(const char *name, const char *regtype, const char *domain,
                        const char *host, uint16_t port, nsd_register_callback_t callback) {

    nsd_register_attrs_t register_attrs = {
            .name = name,
            .regtype = regtype,
            .domain = domain,
            .host = host,
            .port = port,
            .callback = callback
    };

    pthread_t thread;

    /* create a second thread which executes nsd_register() */
    if(pthread_create(&thread, NULL, _nsd_register, &register_attrs)) {
        fprintf(stderr, "pthread_create: failed!\n");
        return -1;
    }

    if(pthread_detach(thread)) {
        fprintf(stderr, "pthread_detach: failed!\n");
        return -1;
    }
    return 0;
}
#endif

static void nsd_browse_main_callback(DNSServiceRef serviceRef,
                                DNSServiceFlags flags,
                                uint32_t interfaceIndex,
                                DNSServiceErrorType errorCode,
                                const char *serviceName,
                                const char *regtype,
                                const char *replyDomain,
                                void *context) {

    if(errorCode == kDNSServiceErr_NoError) {
        printf("nsd service discovered!\n");

        nsd_flags_t nsd_flags = (flags & kDNSServiceFlagsAdd) ? ADDED : REMOVED;
        nsd_flags |= (flags & kDNSServiceFlagsMoreComing) ? MORE : 0;

        if(context) {
            nsd_context_t *nsd_context = ((nsd_context_t *) context);
            nsd_context->callback.browse_callback(interfaceIndex, serviceName, regtype, replyDomain, nsd_flags, nsd_context->context);
        }
    } else {
        fprintf(stderr, "nsd_browse_main_callback: failed! error: %d\n", errorCode);
    }
}

void nsd_browse(const char *regtype, const char *domain, nsd_browse_callback_t callback) {

    DNSServiceRef serviceRef;

    nsd_context_t *nsd_context = malloc(sizeof(nsd_context_t));
    nsd_context->callback_type = BROWSE_CALLBACK;
    nsd_context->callback.browse_callback = callback;

    if(DNSServiceBrowse(&serviceRef,
                     0, // flags - optional settings
                     0, // interfaceIndex, if 0 browses on all interfaces
                     regtype, // service type like "_remotely_click._tcp", more at  http://www.dns-sd.org/ServiceTypes.html
                     domain,  // ex. "local", or can be NULL
                     nsd_browse_main_callback,  // function called asynchronouslly when some network service will be disovered
                     &nsd_context // can pass any param of typy void * to callback function
                    ) == kDNSServiceErr_NoError) {

            nsd_context->context = serviceRef;
            nsd_handle_events(nsd_context);
            nsd_free(serviceRef);
    } else {
        fprintf(stderr, "DNSServiceBrowse: failed!\n");
    }
}

struct nsd_browse_attrs {
    const char *regtype;
    const char *domain;
    nsd_browse_callback_t callback;
};
typedef struct nsd_browse_attrs nsd_browse_attrs_t;

void *_nsd_browse(void *browse_attrs) {

    nsd_browse_attrs_t *ba = (nsd_browse_attrs_t *)browse_attrs;
    nsd_browse(ba->regtype, ba->domain, ba->callback);

    return 0;
}

#ifndef _WIN32
int nsd_spawn_browse(const char *regtype, const char *domain, nsd_browse_callback_t callback) {

    nsd_browse_attrs_t browse_attrs = {
            .regtype = regtype,
            .domain = domain,
            .callback = callback
    };

    pthread_t thread;

    /* create a second thread which executes nsd_browse() */
    if(pthread_create(&thread, NULL, _nsd_browse, &browse_attrs)) {
        fprintf(stderr, "pthread_create: failed!\n");
        return -1;
    }

    if(pthread_detach(thread)) {
        fprintf(stderr, "pthread_detach: failed!\n");
        return -1;
    }
    return 0;
}
#endif

static void nsd_resolve_main_callback(DNSServiceRef serviceRef,
                                      DNSServiceFlags flags,
                                      uint32_t interfaceIndex,
                                      DNSServiceErrorType errorCode,
                                      const char *fullname,
                                      const char *hosttarget,
                                      uint16_t port,
                                      uint16_t txtLen,
                                      const unsigned char *txtRecord,
                                      void *context) {

    if(errorCode == kDNSServiceErr_NoError) {
        printf("nsd service resolved!\n");

        nsd_flags_t nsd_flags = (flags & kDNSServiceFlagsMoreComing) ? MORE : 0;

        if(context) {
            nsd_context_t *nsd_context = ((nsd_context_t *) context);
            nsd_context->callback.resolve_callback(interfaceIndex, fullname, hosttarget, port, nsd_flags, nsd_context->context);
        }

        // IMPORTANT! nsd_free(serviceRef); // to enable later service discovery and do not block thread with handle resolve events loop
    } else {
        fprintf(stderr, "nsd_resolve_main_callback: failed! error: %d\n", errorCode);
    }
}

void nsd_resolve(const char *name, const char *regtype, const char *domain,
                 const uint32_t interface_idx, nsd_resolve_callback_t callback) {

    DNSServiceRef serviceRef;

    nsd_context_t *nsd_context = malloc(sizeof(nsd_context_t));
    nsd_context->callback_type = RESOLVE_CALLBACK;
    nsd_context->callback.resolve_callback = callback;

    if(DNSServiceResolve(&serviceRef,
                         0, // flags - optional settings
                         interface_idx, // discovered in browser or 0 (any interface)
                         name, // as discovered in browse
                         regtype, // as discovered in browse
                         domain, // as discovered in browse
                         nsd_resolve_main_callback, // function called asynchronouslly when network service will be resolved
                         &nsd_context // can pass any param of typy void * to callback function
                        ) == kDNSServiceErr_NoError) {

        nsd_context->context = serviceRef;
        nsd_handle_events(nsd_context);
        nsd_free(serviceRef);
    } else {
        fprintf(stderr, "DNSServiceResolve: failed!\n");
    }
}

struct nsd_resolve_attrs {
    const char *name;
    const char *regtype;
    const char *domain;
    const uint32_t interface_idx;
    nsd_resolve_callback_t callback;
};
typedef struct nsd_resolve_attrs nsd_resolve_attrs_t;

void *_nsd_resolve(void *resolve_attrs) {

    nsd_resolve_attrs_t *ra = (nsd_resolve_attrs_t *)resolve_attrs;
    nsd_resolve(ra->name, ra->regtype, ra->domain, ra->interface_idx, ra->callback);

    return 0;
}

#ifndef WIN32
int nsd_spawn_resolve(const char *name, const char *regtype, const char *domain,
                      const uint32_t interface_idx, nsd_resolve_callback_t callback) {

    nsd_resolve_attrs_t resolve_attrs = {
            .name = name,
            .regtype = regtype,
            .domain = domain,
            .interface_idx = interface_idx,
            .callback = callback
    };

    pthread_t thread;

    /* create a second thread which executes nsd_resolve() */
    if(pthread_create(&thread, NULL, _nsd_resolve, &resolve_attrs)) {
        fprintf(stderr, "pthread_create: failed!\n");
        return -1;
    }

    if(pthread_detach(thread)) {
        fprintf(stderr, "pthread_detach: failed!\n");
        return -1;
    }
    return 0;
}
#endif

void nsd_free(DNSServiceRef serviceRef) {

    DNSServiceRefDeallocate(serviceRef);
}