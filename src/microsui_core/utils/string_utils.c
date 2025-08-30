
#include <string.h>
#include <stdlib.h>
#include <errno.h>

/* Safe copy helpers (always NUL-terminate) */
static void str_copy_safe(char *dst, size_t dst_size, const char *src) {
    if (dst_size == 0) return;
    size_t n = strlen(src);
    if (n >= dst_size) n = dst_size - 1;
    memcpy(dst, src, n);
    dst[n] = '\0';
}

static void mem_copy_safe(char *dst, size_t dst_size, const char *src, size_t src_len) {
    if (dst_size == 0) return;
    if (src_len >= dst_size) src_len = dst_size - 1;
    memcpy(dst, src, src_len);
    dst[src_len] = '\0';
}

/**
 * parse_url
 * Extract host, path and port from a URL like:
 *   http://fullnode.testnet.sui.io:443/
 *   https://example.com
 *   http://10.0.0.5:9000/api
 *   https://[2001:db8::1]:8443/v1
 *
 * Notes:
 * - If no path is present, path is set to "/".
 * - If no port is present, defaults: http=80, https=443, else 0.
 * - host and path are always NUL-terminated (truncated if needed).
 * - Returns 0 on success, non-zero on error.
 */
int parse_url(const char *url,
              char *host, size_t host_size,
              char *path, size_t path_size,
              int *port_out)
{
    if (!url || !host || !path || !port_out || host_size == 0 || path_size == 0) {
        return -1; // invalid args
    }

    const char *p = url;
    int default_port = 0;

    // 1) Detect scheme and set default port
    if (strncmp(p, "http://", 7) == 0) {
        p += 7;
        default_port = 80;
    } else if (strncmp(p, "https://", 8) == 0) {
        p += 8;
        default_port = 443;
    } else {
        default_port = 0; // unknown scheme, leave 0
    }

    // 2) Split authority (host[:port]) and path
    const char *host_begin = p;
    const char *path_begin = strchr(host_begin, '/'); // first slash after host

    // If no path present, set path="/"
    if (!path_begin) {
        str_copy_safe(path, path_size, "/");
    } else {
        // Copy from slash to end as path
        mem_copy_safe(path, path_size, path_begin, strlen(path_begin));
    }

    // 3) Extract host[:port] portion
    const char *host_end = path_begin ? path_begin : (url + strlen(url));

    // Handle IPv6 literal host: [2001:db8::1]:port
    int port = default_port;
    if (host_begin < host_end && *host_begin == '[') {
        // Find closing ']'
        const char *br = memchr(host_begin, ']', (size_t)(host_end - host_begin));
        if (!br) return -2; // malformed IPv6 literal

        // Copy host without brackets
        mem_copy_safe(host, host_size, host_begin + 1, (size_t)(br - (host_begin + 1)));

        // Optional :port after ']'
        const char *after = br + 1;
        if (after < host_end && *after == ':') {
            char port_buf[16];
            size_t plen = (size_t)(host_end - (after + 1));
            if (plen >= sizeof(port_buf)) plen = sizeof(port_buf) - 1;
            memcpy(port_buf, after + 1, plen);
            port_buf[plen] = '\0';
            errno = 0;
            long prt = strtol(port_buf, NULL, 10);
            if (errno != 0 || prt <= 0 || prt > 65535) return -3;
            port = (int)prt;
        }
    } else {
        // Regular hostname / IPv4: host[:port]
        const char *colon = memchr(host_begin, ':', (size_t)(host_end - host_begin));
        if (colon) {
            // Copy host up to colon
            mem_copy_safe(host, host_size, host_begin, (size_t)(colon - host_begin));

            // Parse port after colon
            const char *port_str = colon + 1;
            char port_buf[16];
            size_t plen = (size_t)(host_end - port_str);
            if (plen >= sizeof(port_buf)) plen = sizeof(port_buf) - 1;
            memcpy(port_buf, port_str, plen);
            port_buf[plen] = '\0';
            errno = 0;
            long prt = strtol(port_buf, NULL, 10);
            if (errno != 0 || prt <= 0 || prt > 65535) return -3;
            port = (int)prt;
        } else {
            // No explicit port
            mem_copy_safe(host, host_size, host_begin, (size_t)(host_end - host_begin));
        }
    }

    // 4) If path was absent we already set "/"; ensure host not empty
    if (host[0] == '\0') return -4;

    *port_out = port;
    return 0;
}