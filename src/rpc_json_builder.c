#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "byte_conversions.h"
#include "utils/string_builder.h"

#define SIGNATURE_BYTES_LENGTH 97
#define SIGNATURE_B64_LENGTH 132 // 132 = (97 + 2) / 3 * 4

#define MAX_LENGTH_SUI_MESSAGE 128000 // Maximum length for Sui message in bytes 128k

char* microsui_prepare_executeTransactionBlock(const uint8_t sui_sig[97], const uint8_t* sui_msg, size_t sui_msg_len) {
    const char* execute_method = "sui_executeTransactionBlock";

    // Ensure the signature and message are valid
    if (!sui_sig || !sui_msg || sui_msg_len == 0) {
        return NULL; // Invalid input
    }

    // Prepare the Sui Signature for Base64 encoding
    char sign_base64[SIGNATURE_B64_LENGTH + 1]; // +1 for null terminator
    if (bytes_to_base64(sui_sig, SIGNATURE_BYTES_LENGTH, sign_base64, SIGNATURE_B64_LENGTH + 1) != 0) return NULL; // Base64 encoding failed
    
    // Prepare the Sui Message for Base64 encoding
    size_t msg_base64_len = (sui_msg_len + 2) / 3 * 4;  //TODO Make function for calculate function of this
    char msg_base64[msg_base64_len + 1]; // +1 for null terminator
    if (bytes_to_base64(sui_msg, sui_msg_len, msg_base64, msg_base64_len + 1) != 0) return NULL; // Base64 encoding failed

    // Prepare the JSON-RPC request string - Using a StringBuilder for efficient string concatenation
    StringBuilder sb;
    if (!sb_init(&sb, 1024, MAX_LENGTH_SUI_MESSAGE + 512)) { // Initial size + extra space for JSON
        return NULL;
    }

    sb_append(&sb, "{");
    sb_appendf(&sb, "\"jsonrpc\":\"2.0\",");
    sb_appendf(&sb, "\"id\":1,");
    sb_appendf(&sb, "\"method\":\"%s\",", execute_method);
    sb_appendf(&sb, "\"params\": [");
    sb_appendf(&sb, "\"%s\",", msg_base64);
    sb_appendf(&sb, "[\"%s\"],", sign_base64);
    sb_appendf(&sb, "{");
    sb_appendf(&sb, "\"showInput\":false,");
    sb_appendf(&sb, "\"showRawInput\":false,");
    sb_appendf(&sb, "\"showEffects\":true,");
    sb_appendf(&sb, "\"showEvents\":true,");
    sb_appendf(&sb, "\"showObjectChanges\":true,");
    sb_appendf(&sb, "\"showBalanceChanges\":true,");
    sb_appendf(&sb, "\"showRawEffects\":false}");
    sb_appendf(&sb, ",\"WaitForLocalExecution\"");
    sb_appendf(&sb, "]}");

    return sb_detach(&sb); // caller must free
}