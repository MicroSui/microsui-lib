// Client.h — Public header for MicroSui Client
// Exposes the struct and constructors for use in other files

#ifndef MICROSUI_CLIENT_H
#define MICROSUI_CLIENT_H

#include <stdint.h>
#include <stddef.h>

#include "Keypair.h"
#include "Transaction.h"
#include "microsui_core/rpc_json_decoder.h"

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

#ifndef RPC_RESPONSE_STRUCTS
#define RPC_RESPONSE_STRUCTS

typedef struct BalanceChange {
    char* amount;    // e.g., "100000000"
    char* coinType;  // e.g., "0x2::sui::SUI"
    char* owner;     // e.g., "0xabc..." (flattened AddressOwner if present)
} BalanceChange;

typedef struct SuiTransactionBlockResponse {
    BalanceChange balanceChanges[MAX_BALANCE_CHANGES];
    int           balanceChanges_len;       // actual count

    char* checkpoint;               // may be NULL if not present
    char* confirmedLocalExecution;  // "true"/"false" or NULL
    char* digest;                   // may be NULL if not present

    // Internal arena for string storage (do not use directly from outside)
    char   _arena[RESP_ARENA_SIZE];
    size_t _used;
} SuiTransactionBlockResponse;

#endif

// ==========================
// Constructors
// ==========================
MicroSuiClient SuiClient_newClient(const char* rpc_url);

#endif // MICROSUI_CLIENT_H