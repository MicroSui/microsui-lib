/**
 * @file WiFi.c
 * @brief MicroSui Object-style API for handling WiFi connectivity in MicroSui.
 *
 * This module provides a simple object-oriented style interface
 * (via structs and function pointers) to manage WiFi connections
 * in embedded environments where MicroSui is deployed.
 *
 * Features:
 * - Connect to a WiFi network by SSID and password.
 * - Disconnect from the current WiFi network and release resources.
 *
 * Notes:
 * - Underlying connection logic is handled by microsui_core/wifi_router.h.
 * - Designed for embedded systems (e.g., ESP32/Arduino).
 * - The API mimics an OO-style SDK for clarity and consistency.
 */

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
/**
 * @brief Connect to a WiFi network.
 *
 * Initializes a MicroSuiWiFi instance with the given SSID and password
 * and establishes the connection using the MicroSui core WiFi router.
 *
 * @param[in] ssid      Null-terminated string with the WiFi SSID.
 * @param[in] password  Null-terminated string with the WiFi password.
 *
 * @return Initialized MicroSuiWiFi struct with a disconnect method assigned.
 */
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
/**
 * @brief Disconnect from the current WiFi network.
 *
 * Calls the underlying MicroSui core routine to close the WiFi connection.
 * The MicroSuiWiFi instance remains valid, but no network is active after this call.
 *
 * @param[in] self   Pointer to MicroSuiWiFi instance.
 *
 * @note The parameter is unused in the current implementation but kept
 *       for consistency with the OO-style design and is already defined 
 *       for possible future hardware implementations that may need it.
 */
void ms_disconnect_impl(MicroSuiWiFi *self) {
    (void)self;

    microsui_disconnect_wifi();
}