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
 * @param[in]  pubkey           32-byte Ed25519 public key.
 * @param[out] sui_address_out  Output buffer for the 32-byte Sui address.
 *
 * @return 0 on success; -1 if input pointers are NULL.
 *
 * @note This implementation follows the Sui address derivation convention:
 *       sui_address = blake2b_256([scheme_flag | pubkey]).
 */
int microsui_pubkey_to_sui_address(const uint8_t pubkey[32], uint8_t sui_address_out[32]) {
    if (!pubkey || !sui_address_out) return -1;

    uint8_t input[33];
    input[0] = 0x00; // flag de esquema: Ed25519
    memcpy(input + 1, pubkey, 32);

    // Monocypher: hash_size = 32, msg = input (33 bytes)
    crypto_blake2b(sui_address_out, 32, input, 33);

    return 0;
}

/**
 * @brief Derive a 32-byte Ed25519 public key from a 32-byte private key seed.
 *
 * Generates the full 64-byte Ed25519 secret key internally, then extracts
 * the corresponding public key.
 *
 * @param[in]  private_key  32-byte Ed25519 private key seed.
 * @param[out] public_key   Output buffer for the 32-byte public key.
 *
 * @return 0 on success; -1 if input pointers are NULL.
 *
 * @note The private key is copied to a local buffer before use, to avoid
 *       modifying the caller’s memory.
 */
int get_public_key_from_private_key(const uint8_t private_key[32], uint8_t public_key[32]) {
    if (private_key == NULL || public_key == NULL) return -1;

    // Copy private key to a local variable
    uint8_t private_key_cp[32];
    memcpy(private_key_cp, private_key, 32);

    // Generate public key from private key using Ed25519
    uint8_t secret_key[64];
    crypto_ed25519_key_pair(secret_key, public_key, private_key_cp);
    
    return 0;
}