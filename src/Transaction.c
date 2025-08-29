/**
 * @file Transaction.c
 * @brief MicroSui Object-style API for constructing and managing Sui transactions.
 *
 * This module provides a C object-oriented style interface (via structs and
 * function pointers) to represent Sui transactions. Each transaction holds
 * its raw byte data and exposes methods to build or clear its contents.
 *
 * Features:
 * - Initialize empty transactions.
 * - Create transactions from predefined hex-encoded bytes.
 * - Build transactions into raw byte arrays (placeholder).
 * - Clear and free transaction memory safely.
 *
 * Notes:
 * - Full transaction construction using Sui BCS serialization is **not yet implemented**.
 *   For now, transactions are provided manually as raw TxBytes, and the actual
 *   construction/serialization is delegated externally.
 * - Memory for transaction bytes is dynamically allocated when using
 *   SuiTransaction_setPrebuiltTxBytes(). Always call `clear()` to avoid leaks.
 * - Designed for embedded-friendly environments, following the MicroSui style.
 * 
 * Inspired by the Mysten Labs TypeScript SDK, adapted for embedded C.
 */

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "microsui_core/byte_conversions.h"

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
// Constructor prototypes
// ==========================
MicroSuiTransaction SuiTransaction_init();
MicroSuiTransaction SuiTransaction_setPrebuiltTxBytes(const char *txBytesString);

// ==========================
// Internal method prototypes (implementations)
// ==========================
static TransactionBytes ms_build_impl(MicroSuiTransaction *self);
void ms_clear_impl(MicroSuiTransaction *self);

// ==========================
// Constructor implementations
// ==========================
/**
 * @brief Initialize an empty MicroSuiTransaction.
 *
 * Sets up a transaction with no bytes (data = NULL, length = 0) and
 * assigns method pointers for building and clearing.
 *
 * @return Initialized MicroSuiTransaction struct.
 */
MicroSuiTransaction SuiTransaction_init() {
    MicroSuiTransaction tx;
    memset(&tx, 0, sizeof(tx));

    // Initialize the transaction bytes to an empty state
    tx.tx_bytes.data = NULL;
    tx.tx_bytes.length = 0;

    // Assign methods
    tx.build = ms_build_impl;
    tx.clear = ms_clear_impl;

    return tx;
}

/**
 * @brief Initialize a transaction from a prebuilt hex-encoded TxBytes string.
 *
 * This function is a temporary helper: it assumes the caller already provides
 * the raw transaction bytes (TxBytes) serialized externally, encoded as a
 * hex string. These bytes are then decoded and stored inside the
 * MicroSuiTransaction struct. The transaction takes ownership of the allocated
 * memory, which will be released when `clear()` is called.
 *
 * @param[in] txBytesString   Null-terminated string containing prebuilt TxBytes
 *                            (hex-encoded).
 *
 * @return MicroSuiTransaction struct initialized with the provided TxBytes.
 *
 * @note This is only a placeholder until full Sui BCS transaction construction
 *       is implemented. For now, transactions must be prepared externally and
 *       passed as prebuilt bytes.
 * @note Caller must eventually call `clear()` to free allocated memory.
 */
MicroSuiTransaction SuiTransaction_setPrebuiltTxBytes(const char *txBytesString) {
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
    tx.clear = ms_clear_impl;

    return tx;
}

// ==========================
// Method implementations
// ==========================
/**
 * @brief Build the transaction bytes.
 *
 * Placeholder implementation that currently just returns the internal
 * transaction bytes without performing any serialization.
 *
 * @param[in] self   Pointer to the MicroSuiTransaction instance.
 *
 * @return TransactionBytes struct containing pointer and length of data.
 *
 * @note Full transaction building with Sui BCS serialization is not yet supported.
 *       At this stage, TxBytes are provided manually and construction is delegated externally.
 */
static TransactionBytes ms_build_impl(MicroSuiTransaction *self) {
    // TODO: Implement the logic to build the transaction bytes

    return self->tx_bytes;
}

/**
 * @brief Clear and free memory of a transaction.
 *
 * Frees any dynamically allocated memory for transaction bytes,
 * sets the data pointer to NULL, and resets length to zero.
 *
 * @param[in] self   Pointer to the MicroSuiTransaction instance.
 */
void ms_clear_impl(MicroSuiTransaction *self) {
    if (!self) return;
    if (self->tx_bytes.data) {
        free(self->tx_bytes.data);
        self->tx_bytes.data = NULL;
    }
    self->tx_bytes.length = 0;
}