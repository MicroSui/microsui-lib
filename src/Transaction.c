// Transaction.c — MicroSui Transaction object-style skeleton
// - Inspired by Mysten's Sui TypeScript SDK, but written in C
// - Using function pointers to simulate object methods

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "byte_conversions.h"

// ==========================
// Transaction general structs and constants
// ==========================
typedef struct TransactionBytes TransactionBytes;

struct TransactionBytes {
    uint8_t* data;     // Placeholder for transaction bytes data
    size_t length;      // Length of the transaction bytes
};

// ==========================
// Main struct declaration
// ==========================
typedef struct MicroSuiTransaction MicroSuiTransaction;

struct MicroSuiTransaction {
    TransactionBytes tx_bytes; // Transaction bytes

    // OO-style methods
    const uint8_t* (*build)(MicroSuiTransaction *self);
};

// ==========================
// Constructor prototypes
// ==========================
MicroSuiTransaction SuiTransaction_init();
MicroSuiTransaction SuiTransaction_setHarcodedTxBytes(const char *txBytesString);

// ==========================
// Internal method prototypes (implementations)
// ==========================
static const uint8_t* ms_build_impl(MicroSuiTransaction *self);

// ==========================
// Constructor implementations
// ==========================
MicroSuiTransaction SuiTransaction_init() {
    MicroSuiTransaction tx;
    memset(&tx, 0, sizeof(tx));

    // Initialize the transaction bytes to an empty state
    tx.tx_bytes.data = NULL;
    tx.tx_bytes.length = 0;

    // Assign methods
    tx.build = ms_build_impl;

    return tx;
}

MicroSuiTransaction SuiTransaction_setHarcodedTxBytes(const char *txBytesString) {
    MicroSuiTransaction tx;
    memset(&tx, 0, sizeof(tx));

    if (txBytesString == NULL || strlen(txBytesString) == 0) {
        // If the provided string is empty, return an empty transaction
        tx.tx_bytes.data = NULL;
        tx.tx_bytes.length = 0;
        return tx;
    }

    // Convert the hex string to bytes
    size_t message_len = strlen(txBytesString) / 2; // 2 hex chars = 1 byte
    uint8_t *message = malloc(message_len);
    hex_to_bytes(txBytesString, message, message_len);

    // Initialize the transaction bytes with the provided bytes
    tx.tx_bytes.data = message;
    tx.tx_bytes.length = message_len;


    // Assign methods
    tx.build = ms_build_impl;

    return tx;
}

// ==========================
// Empty method implementations
// ==========================
static const uint8_t* ms_build_impl(MicroSuiTransaction *self) {
    // TODO: Implement the logic to build the transaction bytes

    return self->tx_bytes.data;
}