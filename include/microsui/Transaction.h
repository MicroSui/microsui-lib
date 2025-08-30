// Transaction.h — Public header for MicroSui Transactions
// Exposes the struct and constructors for use in other files

#ifndef MICROSUI_TRANSACTION_H
#define MICROSUI_TRANSACTION_H

#include <stdint.h>
#include <stddef.h>

// ==========================
// Transaction general structs and constants
// ==========================
typedef struct {
    uint8_t* data;      // Placeholder for transaction bytes data
    size_t length;      // Length of the transaction bytes
} TransactionBytes;

// ==========================
// Main struct declaration
// ==========================
typedef struct MicroSuiTransaction MicroSuiTransaction;

struct MicroSuiTransaction {
    TransactionBytes tx_bytes; // Transaction bytes

    // OO-style methods
    TransactionBytes (*build)(MicroSuiTransaction *self);
    void (*clear)(MicroSuiTransaction *self);
};

// ==========================
// Constructors
// ==========================
MicroSuiTransaction SuiTransaction_init();
MicroSuiTransaction SuiTransaction_setPrebuiltTxBytes(const char *txBytesString);

#endif // MICROSUI_TRANSACTION_H