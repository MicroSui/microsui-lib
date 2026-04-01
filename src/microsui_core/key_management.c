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
 * @param[out] sui_address_out   Output buffer for the 32-byte Sui address.
 * @param[in]  public_key        32-byte Ed25519 public key.
 *
 * @return 0 on success; -1 if input pointers are NULL.
 *
 * @note This implementation follows the Sui address derivation convention:
 *       sui_address = blake2b_256([scheme_flag | public_key]).
 */
int microsui_derive_sui_address_ed25519(uint8_t sui_address_out[32], const uint8_t public_key[32]) {
    if (!public_key || !sui_address_out) return -1;

    uint8_t input[33];
    input[0] = 0x00; // flag de esquema: Ed25519
    memcpy(input + 1, public_key, 32);

    // Monocypher: hash_size = 32, msg = input (33 bytes)
    crypto_blake2b(sui_address_out, 32, input, 33);

    return 0;
}

/**
 * @brief Derive a 32-byte Ed25519 public key from a 32-byte seed.
 *
 * Generates the full 64-byte Ed25519 secret key internally, then extracts
 * the corresponding public key.
 *
 * @param[out] public_key_out   Output buffer for the 32-byte public key.
 * @param[in]  seed             32-byte Ed25519 seed.
 *
 * @return 0 on success; -1 if input pointers are NULL.
 *
 * @note The seed is copied to a local buffer before use, 
 *       as the underlying library may modify it during key generation.
 */
int microsui_derive_public_key_ed25519(uint8_t public_key_out[32], const uint8_t seed[32]) {
    if (seed == NULL || public_key_out == NULL) return -1;

    // Copy secret key to a local variable
    uint8_t seed_cp[32];
    memcpy(seed_cp, seed, 32);

    // Generate public key from secret key using Ed25519
    uint8_t _[64];  // Placeholder for the secret key (not used here)
    crypto_ed25519_key_pair(_, public_key_out, seed_cp);

    // Clear sensitive data from memory
    crypto_wipe(seed_cp, sizeof seed_cp);
    crypto_wipe(_, sizeof _);
    
    return 0;
}