#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "lib/monocypher/monocypher.h"

/**
 * @brief Derive a Sui address from a 32-byte Ed25519 public key.
 *
 * Prepends the scheme flag (0x00 for Ed25519) to the public key,
 * then computes the BLAKE2b-256 hash of the 33-byte input.
 * The resulting 32-byte digest is the canonical Sui address.
 *
 * @param[out] sui_address_out  Output buffer for the 32-byte Sui address.
 * @param[in]  pubkey           32-byte Ed25519 public key.
 *
 * @return 0 on success; -1 if input pointers are NULL.
 *
 * @note This implementation follows the Sui address derivation convention:
 *       sui_address = blake2b_256([scheme_flag | pubkey]).
 */
int microsui_derive_sui_address_ed25519(uint8_t sui_address_out[32], const uint8_t pubkey[32]) {
    if (!pubkey || !sui_address_out) return -1;

    uint8_t input[33];
    input[0] = 0x00; // flag de esquema: Ed25519
    memcpy(input + 1, pubkey, 32);

    // Monocypher: hash_size = 32, msg = input (33 bytes)
    crypto_blake2b(sui_address_out, 32, input, 33);

    return 0;
}

/**
 * @brief Derive a 32-byte Ed25519 public key from a 32-byte secret key seed.
 *
 * Generates the full 64-byte Ed25519 secret key internally, then extracts
 * the corresponding public key.
 *
 * @param[out] public_key   Output buffer for the 32-byte public key.
 * @param[in]  secret_key  32-byte Ed25519 secret key seed.
 *
 * @return 0 on success; -1 if input pointers are NULL.
 *
 * @note The secret key is copied to a local buffer before use, to avoid
 *       modifying the caller’s memory.
 */
int microsui_derive_public_key_ed25519(uint8_t public_key[32], const uint8_t secret_key[32]) {
    if (secret_key == NULL || public_key == NULL) return -1;

    // Copy secret key to a local variable
    uint8_t secret_key_cp[32];
    memcpy(secret_key_cp, secret_key, 32);

    // Generate public key from secret key using Ed25519
    uint8_t _[64];  // Placeholder for the full secret key (not used here)
    crypto_ed25519_key_pair(_, public_key, secret_key_cp);
    
    return 0;
}