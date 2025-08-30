#include "microsui_core/http_router.h"

#if !HTTP_SUPPORTED_PLATFORMS
#include <errno.h>
#include <stdio.h>

#pragma message("MicroSui notice: HTTP support isn't available on this platform/board yet. Your build will still compile; any call to MicroSui HTTP functions will simply return NULL. If you need MicroSui HTTP functions here, you can add a small adapter for your board to the `microsui-lib`.")


char* microsui_http_post(const char* host,
                         const char* path,
                         int         port,
                         const char* jsonBody)
{
    (void)host; (void)path; (void)port; (void)jsonBody;

    /* One-time friendly runtime message */
    static int warned = 0;
    if (!warned) {
        warned = 1;
        /* Use stderr on desktop; harmless on most systems. */
        fprintf(stderr,
            "MicroSui HTTP: this platform isn't supported yet.\n"
            "To use it here, add an adapter for your platform and save it in src/impl/http/http_<your_platform>.extension.\n");
    }

    errno = ENOSYS; /* Not implemented */
    return NULL;
}


#endif