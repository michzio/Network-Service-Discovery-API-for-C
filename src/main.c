#include <stdio.h>
#include "../include/network_service_discovery.h"
#include "../apple/event_loop.h"

#ifndef _WIN32
    #include <arpa/inet.h>
#endif

void resolve_callback(uint32_t interface_idx,
                      const char *fullname,
                      const char *hosttarget,
                      uint16_t port,
                      nsd_flags_t flags,
                      void *context) {

    printf("RESOLVED: %d | %s | %s | %d ", interface_idx, fullname, hosttarget, port);

    if( !(flags & MORE) )
        fflush(stdout);
}

void browse_callback(uint32_t interface_idx,
                         const char *service_name,
                         const char *regtype,
                         const char *domain,
                         nsd_flags_t flags,
                        void *context) {

    if(flags & ADDED) {
        printf("NSD ADDED: %d | %s | %s | %s\n", interface_idx, service_name, regtype, domain);
    } else if(flags & REMOVED) {
        printf("NSD REMOVED:  %d | %s | %s | %s\n", interface_idx, service_name, regtype, domain);
    }

    if( !(flags & MORE) )
        fflush(stdout);
}

void register_callback(const char *name, const char *regtype, const char *domain, nsd_flags_t flags, void *context) {

    printf("REGISTERED: %s | %s | %s\n", name, regtype, domain);

    //nsd_state_t *nsd_state = (nsd_state_t *) context;
}

int main() {
    printf("Network Service Discovery API\n");



    //nsd_simple_register("_remotely_click._tcp", htons(59600), register_callback);
    //nsd_register("My Remotely", "_remotely_click._tcp", "", "", 6699, register_callback);
    //nsd_spawn_simple_register("_remotely_click._tcp", htons(59600), register_callback);
    nsd_spawn_register("My Remotely", "_remotely_click._tcp", "", "", 6699, register_callback);
    //nsd_browse("_remotely_click._tcp", NULL, browse_callback);
    //nsd_spawn_browse("_remotely_click._tcp", NULL, browse_callback);
    //nsd_resolve("MyRemotelyClick", "_remotely_click._tcp", "local", 0, resolve_callback);
    //nsd_spawn_resolve("Remotely.Click2", "_remotely_click._tcp", "local", 0, resolve_callback);

    sleep(10);

    return 0;
}

