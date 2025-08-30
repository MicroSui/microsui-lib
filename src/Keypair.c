/**
 * @file Keypair.c
 * @brief MicroSui Object-style API for handling Ed25519 keypairs in MicroSui.
 *
 * This module provides a lightweight, object-oriented style interface
 * (using function pointers inside structs) to work with Ed25519 keypairs
 * in the context of the Sui blockchain. 
 *
 * Features:
 * - Generate random keypairs from entropy.
 * - Initialize a keypair from a Bech32-encoded secret key string.
 * - Sign transaction messages with Ed25519.
 * - Retrieve the secret key (Bech32), public key (raw bytes), and
 *   derive the Sui-formatted address.
 *
 * Notes:
 * - Functions returning strings or buffers use static internal storage.
 *   Results will be overwritten by subsequent calls and are not thread-safe.
 * - All cryptographic primitives are delegated to the MicroSui core modules.
 *
 * Inspired by the Mysten Labs TypeScript SDK, adapted for embedded C.
 */

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "microsui_core/sign.h"
#include "microsui_core/cryptography.h"
#include "microsui_core/key_management.h"
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
/**
 * @brief Generate a new Ed25519 keypair for Sui.
 *
 * Creates a MicroSuiEd25519 instance with a random secret key derived
 * from the current system time and an extra seed value.
 * Function pointers for signing, retrieving keys, and deriving the
 * Sui address are also assigned.
 *
 * @param[in] seed_extra   Extra seed mixed with current time to initialize PRNG.
 *
 * @return Initialized MicroSuiEd25519 struct.
 */
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

/**
 * @brief Initialize a MicroSuiEd25519 keypair from a Bech32 secret key string.
 *
 * Decodes the provided Bech32 private key string and stores it in the struct.
 * If the input is invalid or decoding fails, an empty struct is returned.
 * Function pointers for signing, retrieving keys, and deriving the
 * Sui address are also assigned.
 *
 * @param[in] sk   Bech32-encoded secret key string.
 *
 * @return Initialized MicroSuiEd25519 struct, or empty if decoding fails.
 */
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
/**
 * @brief Sign a transaction message with the Ed25519 private key.
 *
 * Converts a hex-encoded message string to bytes, signs it with Ed25519,
 * and encodes the signature into Base64 format.
 *
 * @param[in] self   Pointer to MicroSuiEd25519 instance.
 * @param[in] msg    Hex-encoded transaction message string.
 *
 * @return SuiSignature struct containing both raw bytes and Base64 string.
 */
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

/**
 * @brief Get the secret key in Bech32 string format.
 *
 * Encodes the internal 32-byte secret key into a Bech32-encoded string.
 * The returned string is stored in a static buffer and will be overwritten
 * by subsequent calls.
 *
 * @param[in] self   Pointer to MicroSuiEd25519 instance.
 *
 * @return Pointer to a static null-terminated string containing the secret key.
 */
static const char* ms_getSecretKey_impl(MicroSuiEd25519 *self) {
    static char secret_key[PK_BECH32_LEN + 1]; // Placeholder for secret key
    microsui_encode_sui_privkey(self->secret_key, secret_key);

    return secret_key; // placeholder
}

/**
 * @brief Get the public key derived from the private key.
 *
 * Derives the 32-byte Ed25519 public key corresponding to the internal secret key.
 * The returned buffer is static and will be overwritten by subsequent calls.
 *
 * @param[in] self   Pointer to MicroSuiEd25519 instance.
 *
 * @return Pointer to a static 32-byte array containing the public key.
 */
static const uint8_t* ms_getPublicKey_impl(MicroSuiEd25519 *self) {
    static uint8_t public_key[32]; // Placeholder for Sui address
    get_public_key_from_private_key(self->secret_key, public_key);

    return public_key; // placeholder
}

/**
 * @brief Derive the Sui address from the Ed25519 public key.
 *
 * Computes the public key from the secret key, encodes it into a Sui address,
 * and formats it as a hex string with the "0x" prefix.
 * The returned string is stored in a static buffer and will be overwritten
 * by subsequent calls.
 *
 * @param[in] self   Pointer to MicroSuiEd25519 instance.
 *
 * @return Pointer to a static null-terminated string containing the Sui address.
 */
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