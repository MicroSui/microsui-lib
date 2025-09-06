#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include "byte_conversions.h"
#include "lib/monocypher/monocypher.h"
#include "lib/compact25519/compact_ed25519.h"

/**
 * @brief Sign a Sui Transaction message using Ed25519 and produce a Sui-formatted signature.
 *
 * Builds the "message with intent" (prefix + tx bytes), digests it with BLAKE2b,
 * signs the digest with Ed25519, and encodes the result in the Sui signature
 * format (scheme byte + 64-byte Ed25519 signature + 32-byte public key).
 *
 * @param[out] sui_sig       Output buffer for the Sui signature (must be 97 bytes).
 * @param[in]  message       Pointer to raw transaction bytes (already serialized).
 * @param[in]  message_len   Length of the transaction bytes.
 * @param[in]  private_key   32-byte Ed25519 private key seed.
 *
 * @return 0 on success, negative value on error.
 *
 * @note The resulting signature is encoded as:
 *       [0x00 scheme | 64-byte signature | 32-byte public key].
 * @note This function is specific to the Ed25519 scheme.
 */
int microsui_sign_ed25519(uint8_t sui_sig[97], const uint8_t* message, const size_t message_len, const uint8_t private_key[32]) {
    // 1. Copy private key to a local variable
    uint8_t private_key_cp[32];
    memcpy(private_key_cp, private_key, 32);

    // 2. Generate public and secret key
    uint8_t public_key[32];
    uint8_t secret_key[64];
    crypto_ed25519_key_pair(secret_key, public_key, private_key_cp);

    // 3. Generate digest using BLAKE2b with the message with the intent
    uint8_t digest[32];

    crypto_blake2b_ctx ctx;
    crypto_blake2b_init(&ctx, 32);
    
    const uint8_t intent[3] = {0x00, 0x00, 0x00};
    crypto_blake2b_update(&ctx, intent, sizeof intent);
    crypto_blake2b_update(&ctx, message, message_len);
    
    crypto_blake2b_final(&ctx, digest);

    // 4. Sign the digest using Ed25519 with the private key and public key
    uint8_t ed25519_signature[64];
    compact_ed25519_sign(ed25519_signature, secret_key, digest, 32);

    // 5. Build Sui signature
    sui_sig[0] = 0x00;  // Ed25519 Scheme
    memcpy(sui_sig + 1, ed25519_signature, 64);
    memcpy(sui_sig + 65, public_key, 32);

    crypto_wipe(secret_key, sizeof secret_key);
    crypto_wipe(private_key_cp, sizeof private_key_cp);
    crypto_wipe(&ctx, sizeof ctx);
    crypto_wipe(digest, sizeof digest);

    return 0;
}

/**
 * @brief Generic signing entry point for multiple signature schemes.
 *
 * Dispatches to the appropriate signing routine depending on the provided
 * scheme identifier. Currently only Ed25519 (0x00) is implemented.
 *
 * @param[in]  scheme        Identifier of the signing scheme (0x00 = Ed25519).
 * @param[out] sui_sig       Output buffer for the Sui signature (must be 97 bytes).
 * @param[in]  message       Pointer to the message/transaction bytes.
 * @param[in]  message_len   Length of the message in bytes.
 * @param[in]  private_key   32-byte private key seed (scheme dependent).
 *
 * @return 0 on success if scheme is supported,
 *         -1 if the scheme is not implemented or unsupported.
 *
 * @note Supported schemes:
 *       - 0x00: Ed25519 (implemented).
 *       - Others (Secp256k1, Secp256r1, Multisig, zkLogin, Passkey) are not yet implemented.
 */
int microsui_sign(uint8_t scheme, uint8_t sui_sig[97], const uint8_t* message, const size_t message_len, const uint8_t private_key[32]) {
    switch (scheme) {
        case 0x00: // Pure Ed25519
            return microsui_sign_ed25519(sui_sig, message, message_len, private_key);
        case 0x01: // ECDSA Secp256k1
            fprintf(stderr, "Error: ECDSA Secp256k1 signing is not implemented yet in MicroSui.\n");
            return -1; // Not implemented
        case 0x02: // ECDSA Secp256r1
            fprintf(stderr, "Error: ECDSA Secp256r1 signing is not implemented yet in MicroSui.\n");
            return -1; // Not implemented
        case 0x03: // multisig
            fprintf(stderr, "Error: Multisig signing is not implemented yet in MicroSui.\n");
            return -1; // Not implemented
        case 0x05: // zkLogin
            fprintf(stderr, "Error: zkLogin signing is not implemented yet in MicroSui.\n");
            return -1; // Not implemented
        case 0x06: // passkey
            fprintf(stderr, "Error: Passkey signing is not implemented yet in MicroSui.\n");
            return -1; // Not implemented
        default:
            return -1; // Unsupported scheme
    }
}


///  DEPRECATED FUNCTIONS  ///
/**
 * @brief [DEPRECATED] Use microsui_sign() or microsui_sign_ed25519() instead.
 *
 * @deprecated This function is kept only for backward compatibility.
 *             Please use microsui_sign() or microsui_sign_ed25519(),
 *             which provide better compatibility and performance.
 *
 * @param[out] sui_sig       Output buffer for the Sui signature (must be 97 bytes).
 * @param[in]  message_hex   Null-terminated string containing the full serialized tx message in hexa form.
 * @param[in]  private_key   32-byte Ed25519 private key seed.
 *
 * @return 0 on success, negative value on error.
 */
int microsui_sign_message(uint8_t sui_sig[97], const char* message_hex, const uint8_t private_key[32]) {
    // Convert the hex message to binary bytes
    size_t msg_len = strlen(message_hex) / 2;  // 2 hex chars = 1 byte
    uint8_t* message = (uint8_t*)malloc(msg_len);
    hex_to_bytes(message_hex, message, msg_len);

    // Call the new ED25519 sign function version
    int res = microsui_sign_ed25519(sui_sig, message, msg_len, private_key);

    free(message);
    return res;
}