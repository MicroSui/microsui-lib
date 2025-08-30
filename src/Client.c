/**
 * @file Client.c
 * @brief MicroSui Object-style RPC client for interacting with a Sui fullnode.
 *
 * Provides a small OO-like API (struct + function pointers) to talk to a Sui
 * RPC endpoint over HTTP(S). The current focus is submitting transactions,
 * but this module is designed to expand into a general-purpose blockchain
 * client for data queries as well.
 *
 * Current capabilities:
 * - Bind a client to a specific RPC URL.
 * - Sign-and-execute a transaction (keypair + tx bytes).
 * - Execute a transaction with a precomputed signature.
 *
 * Planned/next capabilities (API surface will grow here):
 * - Read-only blockchain queries (e.g., objects, balances, events, checkpoints).
 * - Account and coin queries (addresses, owned objects/coins).
 * - Transaction/epoch/state inspection and pagination helpers.
 *
 * Notes:
 * - HTTP and JSON (encode/decode) are delegated to microsui_core.
 * - Temporary buffers allocated during requests are freed before returning.
 * - The response structure is populated by the JSON decoder; callers should
 *   not free its internal pointers unless explicitly documented by that decoder.
 *
 * Inspired by the Mysten Labs TypeScript SDK, adapted for embedded C.
 */

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
/**
 * @brief Create a new MicroSuiClient bound to an RPC URL.
 *
 * Initializes the client struct, safely copies the provided URL into an
 * internal buffer, and assigns method pointers.
 *
 * @param[in] rpc_url  Null-terminated RPC endpoint URL (e.g. "https://host:443/").
 *
 * @return Initialized MicroSuiClient struct.
 *
 * @note No dynamic memory is retained by the client object itself.
 */
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
/**
 * @brief Sign a transaction and execute it via the Sui RPC.
 *
 * Converts transaction bytes to hex, asks the keypair to sign the message,
 * builds the JSON RPC request, performs the HTTP POST, and decodes the JSON
 * response into the provided response structure.
 *
 * @param[in] self  Pointer to client instance (must contain a valid rpc_url).
 * @param[in] kp    Keypair used to sign the transaction bytes.
 * @param[in] tx    Transaction holding raw bytes to be signed/executed.
 *
 * @return SuiTransactionBlockResponse with fields populated from the RPC result.
 *
 * @note Temporary buffers (hex string, JSON request, HTTP response body) are
 *       allocated and freed within this function to avoid leaks.
 */
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

/**
 * @brief Execute a transaction with a precomputed signature via the Sui RPC.
 *
 * Builds the JSON RPC request from the provided transaction bytes and signature,
 * performs the HTTP POST, and decodes the JSON response into the response
 * structure.
 *
 * @param[in] self       Pointer to client instance (must contain a valid rpc_url).
 * @param[in] txBytes    Raw transaction bytes to submit.
 * @param[in] signature  Signature structure (raw bytes and/or textual form).
 *
 * @return SuiTransactionBlockResponse with fields populated from the RPC result.
 *
 * @note Temporary buffers (JSON request, HTTP response body) are allocated and
 *       freed within this function to avoid leaks.
 */
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