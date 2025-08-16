#if defined(_WIN32) || defined(__linux__) || defined(__APPLE__) || defined(__unix__)
// impl/http_post_posix.inc
// POSIX (Linux/macOS/*BSD) using libcurl for HTTPS. No OpenSSL in your code.
// Link with: -lcurl

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

// Internal write buffer for libcurl callback
typedef struct {
    char*  out;      // user buffer
    size_t cap;      // capacity (maxLen)
    size_t len;      // current length
} http_buf_t;

static size_t http_write_cb(char* ptr, size_t size, size_t nmemb, void* userdata) {
    http_buf_t* hb = (http_buf_t*)userdata;
    size_t n = size * nmemb;

    if (hb->len >= hb->cap) return n; // already full; discard
    size_t free_space = (hb->cap > hb->len) ? (hb->cap - hb->len - 1) : 0; // leave room for '\0'
    if (n > free_space) n = free_space;

    if (n > 0) {
        memcpy(hb->out + hb->len, ptr, n);
        hb->len += n;
        hb->out[hb->len] = '\0';
    }
    return size * nmemb; // tell curl we "handled" all bytes (even if truncated)
}

// Returns 0 on success; <0 on error.
// Writes only the HTTP body to `responseOut` (null-terminated). Truncates if needed.
int http_post(const char* host,
              const char* path,
              int         port,
              const char* jsonBody,
              char*       responseOut,
              size_t      maxLen)
{
    if (!host || !path || !jsonBody || !responseOut || maxLen == 0) return -100;
    responseOut[0] = '\0';

    // Build full URL: https://host[:port]/path
    // Path is expected to start with '/', but we handle both cases defensively
    char url[1024];
    const char* scheme = (port == 80) ? "http" : "https";
    if (port == 80 || port == 443) {
        snprintf(url, sizeof(url), "%s://%s%s%s",
                 scheme, host, (path[0] == '/' ? "" : "/"), path);
    } else {
        snprintf(url, sizeof(url), "%s://%s:%d%s%s",
                 scheme, host, port, (path[0] == '/' ? "" : "/"), path);
    }

    CURLcode rc;
    static int curl_inited = 0;
    if (!curl_inited) {
        rc = curl_global_init(CURL_GLOBAL_DEFAULT);
        if (rc != CURLE_OK) return -101;
        curl_inited = 1;
    }

    CURL* curl = curl_easy_init();
    if (!curl) return -102;

    // Prepare headers
    struct curl_slist* headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");

    // Output buffer setup
    http_buf_t hb = { .out = responseOut, .cap = maxLen, .len = 0 };
    responseOut[0] = '\0';

    // Basic options
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonBody);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(jsonBody));

    // Timeouts
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, HTTP_POST_CONNECT_TIMEOUT_SEC);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT,        HTTP_POST_TOTAL_TIMEOUT_SEC);

    // HTTPS verification ON by default (good for prod). If you need to disable:
    // curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    // curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

    // Write callback to capture only the body
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, http_write_cb);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &hb);

    // (Optional) Force HTTP/1.1 to avoid chunking quirks:
    // curl_easy_setopt(curl, CURLOPT_HTTP_VERSION, (long)CURL_HTTP_VERSION_1_1);

    rc = curl_easy_perform(curl);

    long http_code = 0;
    if (rc == CURLE_OK) {
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    if (rc != CURLE_OK) {
        // map some common curl errors
        switch (rc) {
            case CURLE_COULDNT_RESOLVE_HOST: return -103;
            case CURLE_COULDNT_CONNECT:      return -104;
            case CURLE_OPERATION_TIMEDOUT:   return -105;
            default:                         return -106;
        }
    }

    // Treat non-2xx as error if you want (optional). Here we just return 0.
    // if (http_code < 200 || http_code >= 300) return -107;

    // Ensure null-termination
    if (hb.len >= hb.cap) responseOut[hb.cap - 1] = '\0';
    return 0;
}

#endif
