//
// Created by Michal Ziobro on 22/05/2017.
//

#include "network_service_discovery.h"
#include <errno.h>
#include <stdio.h>

#define LONG_TIME 100000000

static volatile int time_out = LONG_TIME;

struct nsd_register_callback_wrapper {
    nsd_register_callback_t callback;
};
typedef struct nsd_register_callback_wrapper nsd_register_callback_wrapper_t;

static void nsd_register_main_callback(DNSServiceRef serviceRef,
                                       DNSServiceFlags flags,
                                       DNSServiceErrorType errorCode,
                                       const char *name,
                                       const char *regtype,
                                       const char *domain,
                                       void *context) {

    if(errorCode == kDNSServiceErr_NoError) {
        printf("nsd service registered!\n");

        if(context)
            ((nsd_register_callback_wrapper_t *)context)->callback(name,regtype,domain, 0);

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
}

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

void nsd_register(const char *name, const char *regtype, const char *domain,
                  const char *host, uint16_t port, nsd_register_callback_t callback) {

    DNSServiceRef serviceRef;

    nsd_register_callback_wrapper_t callback_wrapper = { .callback = callback };

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
                          &callback_wrapper
                         ) == kDNSServiceErr_NoError) {
        nsd_handle_events(serviceRef);
        nsd_free(serviceRef);
    }
}

void *_nsd_register(void *register_attrs) {

    nsd_register_attrs_t *ra = (nsd_register_attrs_t *)register_attrs;
    nsd_register(ra->name, ra->regtype, ra->domain, ra->host, ra->port, ra->callback);
}

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

struct nsd_browse_callback_wrapper {
    nsd_browse_callback_t callback;
};
typedef struct nsd_browse_callback_wrapper nsd_browse_callback_wrapper_t;

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

        if(context)
            ((nsd_browse_callback_wrapper_t *)context)->callback(interfaceIndex, serviceName, regtype, replyDomain, nsd_flags);
    } else {
        fprintf(stderr, "nsd_browse_main_callback: failed! error: %d\n", errorCode);
    }
}

void nsd_browse(const char *regtype, const char *domain, nsd_browse_callback_t callback) {

    DNSServiceRef serviceRef;

    nsd_browse_callback_wrapper_t callback_wrapper = { .callback = callback };

    if(DNSServiceBrowse(&serviceRef,
                     0, // flags - optional settings
                     0, // interfaceIndex, if 0 browses on all interfaces
                     regtype, // service type like "_remotely_click._tcp", more at  http://www.dns-sd.org/ServiceTypes.html
                     domain,  // ex. "local", or can be NULL
                     nsd_browse_main_callback,  // function called asynchronouslly when some network service will be disovered
                     &callback_wrapper // can pass any param of typy void * to callback function
                    ) == kDNSServiceErr_NoError) {

            nsd_handle_events(serviceRef);
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
}

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

struct nsd_resolve_callback_wrapper {
    nsd_resolve_callback_t callback;
};
typedef struct nsd_resolve_callback_wrapper nsd_resolve_callback_wrapper_t;

static void nsd_resolve_main_callback(DNSServiceRef serviceRef,
                                      DNSServiceFlags flags,
                                      uint32_t interfaceIndex,
                                      DNSServiceErrorType errorCode,
                                      const char *fullname,
                                      const char *hosttarget,
                                      uint16_t port,
                                      uint16_t txtLen,
                                      const char *txtRecord,
                                      void *context) {

    if(errorCode == kDNSServiceErr_NoError) {
        printf("nsd service resolved!\n");

        nsd_flags_t nsd_flags = (flags & kDNSServiceFlagsMoreComing) ? MORE : 0;

        if(context)
            ((nsd_resolve_callback_wrapper_t *)context)->callback(interfaceIndex, fullname, hosttarget, port, nsd_flags);

        nsd_free(serviceRef); // to enable later service discovery and do not block thread with handle resolve events loop

    } else {
        fprintf(stderr, "nsd_resolve_main_callback: failed! error: %d\n", errorCode);
    }
}

void nsd_resolve(const char *name, const char *regtype, const char *domain,
                 const uint32_t interface_idx, nsd_resolve_callback_t callback) {

    DNSServiceRef serviceRef;

    nsd_resolve_callback_wrapper_t callback_wrapper = { .callback = callback };

    if(DNSServiceResolve(&serviceRef,
                         0, // flags - optional settings
                         interface_idx, // discovered in browser or 0 (any interface)
                         name, // as discovered in browse
                         regtype, // as discovered in browse
                         domain, // as discovered in browse
                         nsd_resolve_main_callback, // function called asynchronouslly when network service will be resolved
                         &callback_wrapper // can pass any param of typy void * to callback function
                        ) == kDNSServiceErr_NoError) {

        nsd_handle_events(serviceRef);
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
}

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

void nsd_handle_events(DNSServiceRef serviceRef) {

    // get file descriptor of DNS service for the Unix Domain Socket
    // used to communicate with the mdnsd deamon running in the background
    int dns_sd_fd = DNSServiceRefSockFD(serviceRef);
    int nfds = dns_sd_fd + 1;
    fd_set read_fds;
    struct timeval tv;

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


void nsd_free(DNSServiceRef serviceRef) {

    DNSServiceRefDeallocate(serviceRef);
}