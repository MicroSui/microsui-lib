// Keypair.h — Public header for MicroSuiEd25519
// Exposes the struct and constructors for use in other files

#ifndef MICROSUI_KEYPAIR_H
#define MICROSUI_KEYPAIR_H

#include <stdint.h>
#include <stddef.h>

// ==========================
// Transaction general structs and constants
// ==========================
typedef struct {
    uint8_t bytes[97];      // Placeholder for the signature in bytes format
    char signature[133];    // Placeholder for the signature in base64 format (132 + null terminator)
} SuiSignature;

// ==========================
// Main struct declaration
// ==========================
typedef struct MicroSuiEd25519 MicroSuiEd25519;

struct MicroSuiEd25519 {
    uint8_t secret_key[32];

    // OO-style methods
    SuiSignature (*signTransaction)(MicroSuiEd25519 *self, const char *msg);
    const char* (*getSecretKey)(MicroSuiEd25519 *self);
    const uint8_t* (*getPublicKey)(MicroSuiEd25519 *self);
    const char* (*toSuiAddress)(MicroSuiEd25519 *self);
};

// ==========================
// Constructors
// ==========================
MicroSuiEd25519 SuiKeypair_generate(uint8_t seed_extra);
MicroSuiEd25519 SuiKeypair_fromSecretKey(const char *sk);

#endif // MICROSUI_KEYPAIR_H