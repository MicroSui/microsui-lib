// Client.c — MicroSui Client object-style skeleton
// - Inspired by Mysten's Sui TypeScript SDK, but written in C
// - Using function pointers to simulate object methods

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

#include "Keypair.h"
#include "Transaction.h"
#include "microsui_core/http_router.h"
#include "microsui_core/rpc_json_builder.h"
#include "microsui_core/byte_conversions.h"
#include "microsui_core/rpc_json_decoder.h"
#include "microsui_core/utils/string_utils.h"

// ==========================
// Main struct declaration
// ==========================
typedef struct MicroSuiClient MicroSuiClient;

struct MicroSuiClient {
    char rpc_url[128]; // Placeholder for the URL

    // OO-style methods
    SuiTransactionBlockResponse (*signAndExecuteTransaction)(MicroSuiClient *self, MicroSuiEd25519 kp, MicroSuiTransaction tx);
    SuiTransactionBlockResponse (*executeTransactionBlock)(MicroSuiClient *self, TransactionBytes txBytes, SuiSignature signature);
};

// ==========================
// Constructor prototypes
// ==========================
MicroSuiClient SuiClient_newClient(const char* rpc_url);

// ==========================
// Internal method prototypes (implementations)
// ==========================
static SuiTransactionBlockResponse ms_signAndExecuteTransaction_impl(MicroSuiClient *self, MicroSuiEd25519 kp, MicroSuiTransaction tx);
static SuiTransactionBlockResponse ms_executeTransactionBlock_impl(MicroSuiClient *self, TransactionBytes txBytes, SuiSignature signature);

// ==========================
// Constructor implementations
// ==========================
MicroSuiClient SuiClient_newClient(const char* rpc_url) {
    MicroSuiClient cl;
    memset(&cl, 0, sizeof(cl));

    if (rpc_url != NULL) {
        strncpy(cl.rpc_url, rpc_url, sizeof(cl.rpc_url) - 1);
        cl.rpc_url[sizeof(cl.rpc_url) - 1] = '\0'; // Ensure null-termination
    } else {
        cl.rpc_url[0] = '\0'; // Empty string if NULL
    }

    // Assign methods
    cl.signAndExecuteTransaction = ms_signAndExecuteTransaction_impl;
    cl.executeTransactionBlock = ms_executeTransactionBlock_impl;

    return cl;
}

// ==========================
// Method implementations
// ==========================
static SuiTransactionBlockResponse ms_signAndExecuteTransaction_impl(
    MicroSuiClient *self, 
    MicroSuiEd25519 kp, 
    MicroSuiTransaction tx
) {
    static SuiTransactionBlockResponse res; // Placeholder for Sui Transaction Response

    // Generate signature
    char* tx_bytes_string = (char*)malloc(tx.tx_bytes.length * 2 + 1);
    bytes_to_hex(tx.tx_bytes.data, tx.tx_bytes.length, tx_bytes_string);
    SuiSignature sig = kp.signTransaction(&kp, tx_bytes_string);
    free(tx_bytes_string); // Free allocated memory for temporary hex string

    // Create a JSON body for the request
    char* jsonRequest = microsui_prepare_executeTransactionBlock(sig.bytes, tx.tx_bytes.data, tx.tx_bytes.length);

    // Parse the URL to extract host, path, and port
    char host[90]; char path[38]; int port = -1;
    if (parse_url(self->rpc_url, host, sizeof(host), path, sizeof(path), &port) != 0) {
        return res;
    }

    char* json_res = microsui_http_post(host, path, port, jsonRequest);
    free(jsonRequest);  // Free allocated memory for Json Request after use

    microsui_generate_tx_block_response_from_json(json_res, &res);
    free(json_res);  // Free allocated memory for Json Response after use

    return res; // placeholder
}

static SuiTransactionBlockResponse ms_executeTransactionBlock_impl(
    MicroSuiClient *self, 
    TransactionBytes txBytes, 
    SuiSignature signature
) {
    static SuiTransactionBlockResponse res; // Placeholder for Sui Transaction Response

    // Create a JSON body for the request
    char* jsonRequest = microsui_prepare_executeTransactionBlock(signature.bytes, txBytes.data, txBytes.length);

    // Parse the URL to extract host, path, and port
    char host[90]; char path[38]; int port = -1;
    if (parse_url(self->rpc_url, host, sizeof(host), path, sizeof(path), &port) != 0) {
        return res;
    }

    char* json_res = microsui_http_post(host, path, port, jsonRequest);
    free(jsonRequest);  // Free allocated memory for Json Request after use

    microsui_generate_tx_block_response_from_json(json_res, &res);
    free(json_res);  // Free allocated memory for Json Response after use

    return res; // placeholder
}