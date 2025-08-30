#ifndef MICROSUI_H
#define MICROSUI_H
/* MicroSui.h is the main header file for the Arduino MicroSui library */

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>

// OO-style headers
#include "Keypair.h"
#include "Transaction.h"
#include "Client.h"
#include "WiFi.h"

// MicroSui core headers
#include "microsui_core/sign.h"
#include "microsui_core/cryptography.h"
#include "microsui_core/key_management.h"
#include "microsui_core/byte_conversions.h"
#include "microsui_core/rpc_json_builder.h"
#include "microsui_core/rpc_json_decoder.h"
#include "microsui_core/http_router.h"
#include "microsui_core/wifi_router.h"

// Utility headers
#include "microsui_core/utils/string_builder.h"
#include "microsui_core/utils/string_utils.h"

#ifdef __cplusplus
}
#endif

#endif
