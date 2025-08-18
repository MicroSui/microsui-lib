
#include "wifi_router.h"

#if !WIFI_SUPPORTED_PLATFORMS
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
    return NULL;
}


#endif