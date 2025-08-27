// Keypair.c — MicroSuiEd25519 object-style skeleton
// - Inspired by Mysten's Sui TypeScript SDK, but written in C
// - Using function pointers to simulate object methods

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "microsui_core/sign.h"
#include "microsui_core/cryptography.h"
#include "microsui_core/byte_conversions.h"

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
// Constructor prototypes
// ==========================
MicroSuiEd25519 SuiKeypair_generate(uint8_t seed_extra);
MicroSuiEd25519 SuiKeypair_fromSecretKey(const char *sk);

// ==========================
// Internal method prototypes (implementations)
// ==========================
static SuiSignature ms_signTransaction_impl(MicroSuiEd25519 *self, const char *msg);
static const char* ms_getSecretKey_impl(MicroSuiEd25519 *self);
static const uint8_t* ms_getPublicKey_impl(MicroSuiEd25519 *self);
static const char* ms_toSuiAddress_impl(MicroSuiEd25519 *self);

// ==========================
// Constructor implementations
// ==========================
MicroSuiEd25519 SuiKeypair_generate(uint8_t seed_extra) {
    MicroSuiEd25519 kp;
    memset(&kp, 0, sizeof(kp));

    // Generate a random secret key
    unsigned seed = (unsigned)time(NULL) ^ seed_extra;
    srand(seed);
    for (size_t i = 0; i < 32; i++) {
        kp.secret_key[i] = (uint8_t)(rand() % 256);
    }

    // Assign methods
    kp.signTransaction = ms_signTransaction_impl;
    kp.getSecretKey    = ms_getSecretKey_impl;
    kp.getPublicKey    = ms_getPublicKey_impl;
    kp.toSuiAddress    = ms_toSuiAddress_impl;

    return kp;
}

MicroSuiEd25519 SuiKeypair_fromSecretKey(const char *sk) {
    MicroSuiEd25519 kp;
    memset(&kp, 0, sizeof(kp));

    if (sk == NULL || strlen(sk) == 0 || strlen(sk) != PK_BECH32_LEN) return kp; // Error: Invalid secret key: return empty struct

    // Decode the secret key from Bech32
    if (microsui_decode_sui_privkey(sk, kp.secret_key) != 0) return kp; // Error: Decoding failed: return empty struct

    // Assign methods
    kp.signTransaction = ms_signTransaction_impl;
    kp.getSecretKey    = ms_getSecretKey_impl;
    kp.getPublicKey    = ms_getPublicKey_impl;
    kp.toSuiAddress    = ms_toSuiAddress_impl;

    return kp;
}

// ==========================
// Method implementations
// ==========================
static SuiSignature ms_signTransaction_impl(MicroSuiEd25519 *self, const char *msg) {
    SuiSignature sig;

    // decode hex string msg -> bytes
    size_t message_len = strlen(msg) / 2;
    uint8_t message[message_len];
    hex_to_bytes(msg, message, message_len);

    // sign
    microsui_sign_ed25519(sig.bytes, message, message_len, self->secret_key);

    // encode to base64
    bytes_to_base64(sig.bytes, 97, sig.signature, sizeof(sig.signature));

    return sig;
}


static const char* ms_getSecretKey_impl(MicroSuiEd25519 *self) {
    static char secret_key[PK_BECH32_LEN + 1]; // Placeholder for secret key
    microsui_encode_sui_privkey(self->secret_key, secret_key);

    return secret_key; // placeholder
}

static const uint8_t* ms_getPublicKey_impl(MicroSuiEd25519 *self) {
    static uint8_t public_key[32]; // Placeholder for Sui address
    get_public_key_from_private_key(self->secret_key, public_key);

    return public_key; // placeholder
}

static const char* ms_toSuiAddress_impl(MicroSuiEd25519 *self) {
    static char sui_address[67]; // Placeholder for Sui address

    // Obtain the public key from the secret key
    uint8_t public_key[32];
    get_public_key_from_private_key(self->secret_key, public_key);

    // Encode public key to Sui address
    uint8_t encoded_address[32];
    microsui_pubkey_to_sui_address(public_key, encoded_address);

    // Convert the encoded address to a hex string
    char encoded_address_string[65];
    bytes_to_hex(encoded_address, 32, encoded_address_string);

    // Format the Sui address
    sui_address[0] = '0';
    sui_address[1] = 'x';
    memcpy(sui_address + 2, encoded_address_string, 65);
    
    return sui_address; // placeholder
}