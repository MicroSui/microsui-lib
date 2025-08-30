#if defined(_WIN32) || defined(__linux__) || defined(__APPLE__) || defined(__unix__)
// Compile & link with: -lcurl

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <curl/curl.h>

#ifndef HTTP_POST_CONNECT_TIMEOUT_SEC
#define HTTP_POST_CONNECT_TIMEOUT_SEC 10L
#endif

#ifndef HTTP_POST_TOTAL_TIMEOUT_SEC
#define HTTP_POST_TOTAL_TIMEOUT_SEC  15L
#endif

// Dynamic buffer used by libcurl write callback
typedef struct {
    char*  data;
    size_t len;
    size_t cap;
} dynbuf_t;

// Grow-on-demand write callback: appends to dynbuf and keeps it NUL-terminated.
// On OOM, returns 0 so libcurl aborts with CURLE_WRITE_ERROR.
static size_t http_write_cb(char* ptr, size_t size, size_t nmemb, void* userdata) {
    dynbuf_t* db = (dynbuf_t*)userdata;
    size_t n = size * nmemb;
    if (n == 0) return 0;

    // Ensure capacity (keep 1 byte for the trailing '\0')
    if (db->len + n + 1 > db->cap) {
        size_t needed = db->len + n + 1;
        size_t newcap = db->cap ? db->cap : 4096;
        while (newcap < needed) {
            size_t prev = newcap;
            newcap *= 2;               // exponential growth to reduce reallocs
            if (newcap < prev) {       // overflow guard (very unlikely)
                newcap = needed;
                break;
            }
        }
        char* p = (char*)realloc(db->data, newcap);
        if (!p) {
            return 0; // force libcurl to error out
        }
        db->data = p;
        db->cap  = newcap;
    }

    memcpy(db->data + db->len, ptr, n);
    db->len += n;
    db->data[db->len] = '\0';
    return n; // bytes consumed
}

/**
 * Performs HTTP/HTTPS POST and returns the response BODY as a NUL-terminated C string.
 *
 * Success: returns malloc/realloc'd buffer (caller must free()).
 * Error (transport/memory/etc.): returns NULL.
 *
 * Notes:
 *  - If server returns 4xx/5xx, this still returns the body (API has no status out-param).
 *  - Scheme: "http" if port == 80, otherwise "https".
 *  - If port is not 80/443, ":port" is included in the URL.
 */
char* microsui_http_post(const char* host, const char* path, int port, const char* jsonBody) {
    if (!host || !path || !jsonBody) return NULL;

    // Build URL: http(s)://host[:port]/path
    char url[1024];
    const char* scheme = (port == 80) ? "http" : "https";
    int r;
    if (port == 80 || port == 443) {
        r = snprintf(url, sizeof(url), "%s://%s%s%s",
                     scheme, host, (path[0] == '/' ? "" : "/"), path);
    } else {
        r = snprintf(url, sizeof(url), "%s://%s:%d%s%s",
                     scheme, host, port, (path[0] == '/' ? "" : "/"), path);
    }
    if (r < 0 || r >= (int)sizeof(url)) {
        return NULL; // URL truncated or formatting error
    }

    // One-time global init
    CURLcode rc;
    static int curl_inited = 0;
    if (!curl_inited) {
        rc = curl_global_init(CURL_GLOBAL_DEFAULT);
        if (rc != CURLE_OK) return NULL;
        curl_inited = 1;
    }

    CURL* curl = curl_easy_init();
    if (!curl) return NULL;

    // Headers
    struct curl_slist* headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");

    // Dynamic response buffer
    dynbuf_t db = {0}; // data=NULL, len=0, cap=0

    // Basic options
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonBody);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(jsonBody));

    // Timeouts
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, HTTP_POST_CONNECT_TIMEOUT_SEC);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT,        HTTP_POST_TOTAL_TIMEOUT_SEC);

    // HTTPS verification is ON by default (recommended).
    // To disable (NOT recommended in production):
    // curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    // curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

    // Capture only the body
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, http_write_cb);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &db);

    // Optional: avoid signals on POSIX/multi-threaded apps
    // curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);

    rc = curl_easy_perform(curl);

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    if (rc != CURLE_OK) {
        if (db.data) free(db.data);
        return NULL;
    }

    // Ensure we always return a valid buffer (even if empty)
    if (!db.data) {
        db.data = (char*)malloc(1);
        if (!db.data) return NULL;
        db.data[0] = '\0';
    }

    return db.data; // caller must free(ptr)
}

#endif // desktop platforms