// WiFi.c — MicroSui WiFi Client object-style skeleton
// - Inspired by Mysten's Sui TypeScript SDK, but written in C
// - Using function pointers to simulate object methods

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

#include "microsui_core/wifi_router.h"

// ==========================
// Main struct declaration
// ==========================
typedef struct MicroSuiWiFi MicroSuiWiFi;

struct MicroSuiWiFi {
    char ssid[64]; // Placeholder for the WiFi SSID
    char password[64]; // Placeholder for the WiFi Password

    // OO-style methods
    void (*disconnect)(MicroSuiWiFi *self);
};

// ==========================
// Constructor prototypes
// ==========================
MicroSuiWiFi WiFi_connect(const char* ssid, const char* password);

// ==========================
// Internal method prototypes (implementations)
// ==========================
void ms_disconnect_impl(MicroSuiWiFi *self);

// ==========================
// Constructor implementations
// ==========================
MicroSuiWiFi WiFi_connect(const char* ssid, const char* password) {
    MicroSuiWiFi wifi_cl;
    memset(&wifi_cl, 0, sizeof(wifi_cl));

    microsui_connect_wifi(ssid, password);

    // Assign methods
    wifi_cl.disconnect = ms_disconnect_impl;

    return wifi_cl;
}

// ==========================
// Method implementations
// ==========================
void ms_disconnect_impl(MicroSuiWiFi *self) {
    (void)self;

    microsui_disconnect_wifi();
}