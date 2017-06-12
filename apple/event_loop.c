//
// Created by Michal Ziobro on 23/05/2017.
//

#ifdef __APPLE__

#include "event_loop.h"

#include <dns_sd.h>
#include <CoreFoundation/CoreFoundation.h>

// remove Network Service Discovery from CFRunLoop's event sources
void nsd_remove_event_source(CFRunLoopRef runLoopRef, nsd_state_t *nsd_state) {

    if(runLoopRef == NULL) {
        fprintf(stderr, "nsd_remove_event_source: failed! CFRunLoopRef is NULL.\n");
        return;
    }
    if(nsd_state == NULL) {
        fprintf(stderr, "nsd_remove_event_source: failed! nsd_state_t is NULL.\n");
        return;
    }

    // remove CFRunLoopSource from the current run loop.
    CFRunLoopRemoveSource(runLoopRef, nsd_state->sourceRef, kCFRunLoopCommonModes);
    CFRelease(nsd_state->sourceRef);

    // invalidate the CFSocket
    CFSocketInvalidate(nsd_state->cfSocketRef);
    CFRelease(nsd_state->cfSocketRef);

    // workaround to give time to CFSocket's select() thread
    // to remove the socket from its FD set
    usleep(1000);

    // terminate the connection with the deamon
    DNSServiceRefDeallocate(nsd_state->serviceRef);
    free(nsd_state);
}


// helper function invoked asynchronously
// when CFRunLoop encounter new event
static void cfSocketCallBack(CFSocketRef cfSockerRef, CFSocketCallBackType type, CFDataRef dataRef,
                             const void *data, void *info) {

    DNSServiceErrorType err;
    nsd_state_t *nsd_state = (nsd_state_t *) info;
    if(nsd_state == NULL) {
        fprintf(stderr, "cfSocketCallBack: failed! nsd_state is null.\n");
        return;
    }

    if( (err = DNSServiceProcessResult(nsd_state->serviceRef) ) != kDNSServiceErr_NoError) {
        fprintf(stderr, "DNSServiceProcessResult: failed! error: %d\n", err);
        nsd_remove_event_source(CFRunLoopGetCurrent(), nsd_state);
    }
}

// add Network Service Discovery to CFRunLoop's event sources
void nsd_add_event_source(CFRunLoopRef runLoopRef, nsd_state_t *nsd_state) {

    CFSocketContext context = { 0, (void *) nsd_state, NULL, NULL, NULL };
    CFSocketNativeHandle socket;
    if((socket = DNSServiceRefSockFD(nsd_state->serviceRef)) == -1) {
        fprintf(stderr, "DNSServiceRefSockFD: failed!\n");
        return;
    }

    // create a CFSocketRef from Unix Domain socket
    nsd_state->cfSocketRef = CFSocketCreateWithNative(NULL, socket, kCFSocketReadCallBack, cfSocketCallBack, &context);

    // remove flag CFSocketCloseOnInvalidate from socket flags
    CFOptionFlags socketFlags = CFSocketGetSocketFlags(nsd_state->cfSocketRef);
    CFSocketSetSocketFlags(nsd_state->cfSocketRef, socketFlags & ~kCFSocketCloseOnInvalidate);

    // create a CFRunLoopSource from the CFSocket
    nsd_state->sourceRef = CFSocketCreateRunLoopSource(NULL, nsd_state->cfSocketRef, 0);

    // add the CFRunLoopSource to the current run loop.
    CFRunLoopAddSource(runLoopRef, nsd_state->sourceRef, kCFRunLoopCommonModes);

    nsd_state->runLoopRef = runLoopRef;
}

void nsd_handle_events(nsd_context_t *nsd_context) {

    // printf("Apple");
    // fflush(stdout);

    DNSServiceRef serviceRef = (DNSServiceRef) nsd_context->context;
    nsd_state_t *nsd_state = malloc(sizeof(nsd_state_t));
    nsd_state->serviceRef = serviceRef;
    nsd_context->context = nsd_state;

    nsd_add_event_source(CFRunLoopGetCurrent(), nsd_state);

    CFRunLoopRun();
}

#endif // __APPLE__