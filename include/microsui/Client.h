// Client.h — Public header for MicroSui Client
// Exposes the struct and constructors for use in other files

#ifndef MICROSUI_CLIENT_H
#define MICROSUI_CLIENT_H

#include <stdint.h>
#include <stddef.h>

#include "Keypair.h"
#include "Transaction.h"

// ==========================
// Transaction general structs and constants
// ==========================
typedef struct {
    char* jsonResponse;      // Placeholder for json response
} SuiTransactionBlockResponse;

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
// Constructors
// ==========================
MicroSuiClient SuiClient_newClient(const char* rpc_url);

#endif // MICROSUI_CLIENT_H