#include "microsui_core/wifi_router.h"

#if !WIFI_SUPPORTED_PLATFORMS

#pragma message("MicroSui notice: WiFi support isn't available on this platform/board yet. Your build will still compile; any call to MicroSui WiFi functions will simply return NULL. If you need MicroSui WiFi functions here, you can add a small adapter for your board to the `microsui-lib`.")

#include <errno.h>
#include <stdio.h>

int microsui_connect_wifi(const char* ssid, const char* password) 
{
    (void)ssid; (void)password;

    /* One-time friendly runtime message */
    static int warned = 0;
    if (!warned) {
        warned = 1;
        /* Use stderr on desktop; harmless on most systems. */
        fprintf(stderr,
            "MicroSui WiFi: this platform isn't supported yet.\n"
            "To use it here, add an adapter for your platform and save it in src/impl/wifi/wifi_<your_platform>.extension.\n");
    }

    errno = ENOSYS; /* Not implemented */
    return -1;
}

int microsui_disconnect_wifi() {
    /* One-time friendly runtime message */
    static int warned = 0;
    if (!warned) {
        warned = 1;
        /* Use stderr on desktop; harmless on most systems. */
        fprintf(stderr,
            "MicroSui WiFi: this platform isn't supported yet.\n"
            "To use it here, add an adapter for your platform and save it in src/impl/wifi/wifi_<your_platform>.extension.\n");
    }

    errno = ENOSYS; /* Not implemented */
    return -1;
}

#endif