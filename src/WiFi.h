// WiFi.h — Public header for MicroSui WiFi Client
// Exposes the struct and constructors for use in other files

#ifndef MICROSUI_WIFI_H
#define MICROSUI_WIFI_H

#include <stdint.h>
#include <stddef.h>

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

#endif // MICROSUI_WIFI_H