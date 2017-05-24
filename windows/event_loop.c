//
// Created by Michal Ziobro on 24/05/2017.
//
#ifdef _WIN32

#include "event_loop.h"


void nsd_handle_events(DNSServiceRef serviceRef) {

    // window (not visible) used to process DNS-SD events as messages to the window.
    window = CreateWindow(wcex.lpszClassName, wcex.lpszClassName, 0, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, instance, NULL);


    // associate the DNS-SD browser with our window
    if(WSAAsyncSelect( (SOCKET) DNSServiceRefSockFD(gServiceRef),
                                DNSSD_EVENT, FD_READ | FD_CLOSE) != kDNSServiceErr_NoError) {

              fprintf(stderr, "WSAAsyncSelect: failed!\n");
               return;
    }

    // DNS-SD events are dispatched while in this loop
    while(GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // slean up
    WSAAsyncSelect( (SOCKET) DNSServiceRefSockFD(gServiceRef), wind, DNSSD_EVENT, 0);
}

static LRESULT CALLBACK WndProc(HWND inWindow, UINT inMsg, WPARAM inWParam, LPARAM inLParam)
{
    LRESULT result;
    switch(inMsg) {
        case DNSSD_EVENT:
            DNSServiceProcessResult(gServiceRef);
            result = 0;
            break; //...
    }

    return result;
}

#endif