//
// Created by Michal Ziobro on 24/05/2017.
//
#ifdef _WIN32

#include "event_loop.h"
#include <stdio.h>

#define DNS_SD_EVENT (WM_USER + 0x100)

// globals
static DNSServiceRef  gServiceRef = NULL;

static LRESULT CALLBACK WndProc(HWND inWindow, UINT inMsg, WPARAM inWParam, LPARAM inLParam);

void nsd_handle_events(DNSServiceRef serviceRef) {

    HINSTANCE instance;
    WNDCLASSEX wcex;
    HWND  window;
    MSG messege;
    int error;

    gServiceRef = serviceRef;

    // window (not visible) use to process DNS-SD events as messages to the window.
    if( (instance = (HINSTANCE) GetModuleHandle(NULL)) == 0) {
        fprintf(stderr, "GetModuleHandle: failed!\n");
        return;
    }

    wcex.cbSize = sizeof(wcex);
    wcex.style = 0;
    wcex.lpfnWndProc = (WNDPROC) WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = instance;
    wcex.hIcon = NULL;
    wcex.hCursor = NULL;
    wcex.hbrBackground = NULL;
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = TEXT("ZeroconfNSD");
    wcex.hIconSm = NULL;
    RegisterClassEx(&wcex);

    if( (window = CreateWindow(wcex.lpszClassName, wcex.lpszClassName, 0,
                          CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL,
                          instance, NULL) ) == 0 ) {
        fprintf(stderr, "CreateWindow: failed!\n");
    }

    // associate the DNS-SD browser with our window
    if(WSAAsyncSelect( (SOCKET) DNSServiceRefSockFD(gServiceRef),
                       window, DNS_SD_EVENT, FD_READ | FD_CLOSE) != kDNSServiceErr_NoError) {
        fprintf(stderr, "WSAAsyncSelect: failed!\n");
        return;
    }

    // DNS-SD events are dispatched while in this loop
    while(GetMessage(&messege, NULL, 0, 0))
    {
        TranslateMessage(&messege);
        DispatchMessage(&messege);
    }

    // clean up
    WSAAsyncSelect( (SOCKET) DNSServiceRefSockFD(gServiceRef), window, DNS_SD_EVENT, 0);
}

/*
 * Callback for the Windows. DNS-SD events are delivered here.
 */
static LRESULT CALLBACK WndProc(HWND inWindow, UINT inMsg, WPARAM inWParam, LPARAM inLParam)
{
    LRESULT result;
    switch(inMsg) {
        case DNS_SD_EVENT:
            // process DNS-SD event
            if(DNSServiceProcessResult(gServiceRef) != kDNSServiceErr_NoError) {
                result = -1;
            } else {
                result = 0;
            }
            result = 0;
            break;
        default:
            result = DefWindowProc(inWindow, inMsg, inWParam, inLParam);
            break;
    }

    return result;
}



#endif