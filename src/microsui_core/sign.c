#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include "byte_conversions.h"
#include "lib/monocypher/monocypher.h"

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
    crypto_ed25519_sign_sha512(ed25519_signature, secret_key, digest, 32);

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

/**
 * @brief Verify a Sui-formatted Ed25519 signature against a Sui Transaction message.
 *
 * Expects a Sui signature encoded as:
 *   [0x00 scheme | 64-byte Ed25519 signature | 32-byte public key]
 *
 * Rebuilds the Sui "message with intent" (prefix + tx bytes), digests it with
 * BLAKE2b-256, and verifies the Ed25519 signature using the embedded public key.
 *
 * @param[in] sui_sig        Pointer to the Sui signature buffer (must be 97 bytes).
 * @param[in] message        Pointer to raw transaction bytes (already serialized).
 * @param[in] message_len    Length of the transaction bytes.
 *
 * @return 0 if the signature is valid,
 *         -1 if the scheme byte is not Ed25519 (0x00).
 *         -2 if the signature verification fails for a supported scheme
 *
 * @note The "intent" prefix used is {0x00, 0x00, 0x00}, matching the signing routine.
 * @note This function does not recover a public key; it verifies using the public key
 *       embedded inside the Sui signature payload.
 */
int microsui_verify_signature_ed25519(uint8_t sui_sig[97], const uint8_t* message, const size_t message_len) {
    if(sui_sig[0] != 0x00) {
        return -1; // This is not an Ed25519 signature
    }

    uint8_t signature[64];
    memcpy(signature, sui_sig + 1, 64);

    uint8_t public_key[32];
    memcpy(public_key, sui_sig + 65, 32);

    uint8_t digest[32];
    crypto_blake2b_ctx ctx;
    crypto_blake2b_init(&ctx, 32);
    const uint8_t intent[3] = {0x00, 0x00, 0x00};
    crypto_blake2b_update(&ctx, intent, sizeof intent);
    crypto_blake2b_update(&ctx, message, message_len);
    crypto_blake2b_final(&ctx, digest);

    if(crypto_ed25519_check_sha512(signature, public_key, digest, 32) != 0) {
        return -2; // Signature verification failed
    }
    
    return 0; // Signature is valid
}

/**
 * @brief Verify a Sui-formatted Ed25519 signature and validate it against a known public key.
 *
 * Expects a Sui signature encoded as:
 *   [0x00 scheme | 64-byte Ed25519 signature | 32-byte public key]
 *
 * Rebuilds the Sui "message with intent" (prefix + tx bytes), digests it with
 * BLAKE2b-256, verifies the Ed25519 signature using the public key embedded in
 * the signature payload, and then confirms that the embedded public key matches
 * the caller-supplied public key using a constant-time comparison.
 *
 * @param[in] sui_sig        Pointer to the Sui signature buffer (must be 97 bytes).
 * @param[in] message        Pointer to raw transaction bytes (already serialized).
 * @param[in] message_len    Length of the transaction bytes.
 * @param[in] public_key     Pointer to the expected 32-byte Ed25519 public key.
 *
 * @return  0 if the signature is valid and the embedded public key matches @p public_key,
 *         -1 if the scheme byte is not Ed25519 (0x00),
 *         -2 if the signature verification fails,
 *         -3 if the public key embedded in the signature does not match @p public_key.
 *
 * @note The "intent" prefix used is {0x00, 0x00, 0x00}, matching the signing routine.
 * @note The public key comparison is performed in constant time to mitigate timing attacks.
 * @note Signature verification (-2) is checked before public key comparison (-3); a -3 result
 *       therefore implies the signature itself was cryptographically valid.
 */

int microsui_verify_signature_ed25519_with_public_key(uint8_t sui_sig[97], const uint8_t* message, const size_t message_len, uint8_t public_key[32]) {
    if(sui_sig[0] != 0x00) {
        return -1; // This is not an Ed25519 signature
    }

    uint8_t signature[64];
    memcpy(signature, sui_sig + 1, 64);

    uint8_t signature_public_key[32];
    memcpy(signature_public_key, sui_sig + 65, 32);

    uint8_t digest[32];
    crypto_blake2b_ctx ctx;
    crypto_blake2b_init(&ctx, 32);
    const uint8_t intent[3] = {0x00, 0x00, 0x00};
    crypto_blake2b_update(&ctx, intent, sizeof intent);
    crypto_blake2b_update(&ctx, message, message_len);
    crypto_blake2b_final(&ctx, digest);

    if(crypto_ed25519_check_sha512(signature, signature_public_key, digest, 32) != 0) { 
        return -2; // Signature verification failed
    }

    // Check if the public key in the signature matches the provided public key (with timing attacks protection)
    uint8_t diff = 0;
    for (size_t i = 0; i < 32; i++) {
        diff |= signature_public_key[i] ^ public_key[i];
    }
    if (diff != 0) { 
        return -3; // Public key mismatch
    }

    return 0; // Signature is valid and public key matches
}

/**
 * @brief Generic signature verification entry point for multiple signature schemes.
 *
 * Dispatches to the appropriate verification routine based on the scheme byte
 * found in the provided Sui signature (sui_sig[0]).
 *
 * @param[in] sui_sig        Pointer to the Sui signature buffer (must be 97 bytes).
 * @param[in] message        Pointer to the message/transaction bytes (already serialized).
 * @param[in] message_len    Length of the message in bytes.
 *
 * @return 0 if the signature is valid for the detected scheme,
 *         -1 if the scheme is unsupported or not implemented,
 *         -2 if the signature verification fails for a supported scheme
 *
 * @note Currently supported schemes:
 *       - 0x00: Ed25519.
 * @note Not yet implemented: 
 *       - Secp256k1 (0x01), Secp256r1 (0x02), Multisig (0x03), zkLogin (0x05), Passkey (0x06).
 * @see microsui_verify_signature_ed25519()
 * @see microsui_verify_signature_with_public_key() for the variant that also
 *      validates the embedded public key against a caller-supplied expected key.
 */
int microsui_verify_signature(uint8_t sui_sig[97], const uint8_t* message, const size_t message_len) {
    switch (sui_sig[0]) {
        case 0x00: // Ed25519
            return microsui_verify_signature_ed25519(sui_sig, message, message_len);
        default:
            return -1; // Unsupported scheme
    }
}

/**
 * @brief Generic signature verification entry point with expected public key validation.
 *
 * Dispatches to the appropriate verification routine based on the scheme byte
 * found in the provided Sui signature (sui_sig[0]), and additionally confirms
 * that the public key embedded in the signature matches the caller-supplied key.
 *
 * @param[in] sui_sig        Pointer to the Sui signature buffer (must be 97 bytes).
 * @param[in] message        Pointer to the message/transaction bytes (already serialized).
 * @param[in] message_len    Length of the message in bytes.
 * @param[in] public_key     Pointer to the expected 32-byte public key for the detected scheme.
 *
 * @return  0 if the signature is valid and the embedded public key matches @p public_key,
 *         -1 if the scheme byte is unsupported or not yet implemented,
 *         -2 if the signature verification fails for a supported scheme,
 *         -3 if the public key embedded in the signature does not match @p public_key.
 *
 * @note Currently supported schemes:
 *       - 0x00: Ed25519.
 * @note Not yet implemented: 
 *       - Secp256k1 (0x01), Secp256r1 (0x02), Multisig (0x03), zkLogin (0x05), Passkey (0x06).
 * @note The public key comparison is delegated to the scheme-specific routine
 *       and performed in constant time to mitigate timing attacks.
 * @see microsui_verify_signature_ed25519_with_public_key()
 * @see microsui_verify_signature() for the variant that does not validate
 *      the embedded public key against an expected value.
 */
int microsui_verify_signature_with_public_key(uint8_t sui_sig[97], const uint8_t* message, const size_t message_len, uint8_t public_key[32]) {
    switch (sui_sig[0]) {
        case 0x00: // Ed25519
            return microsui_verify_signature_ed25519_with_public_key(sui_sig, message, message_len, public_key);
        default:
            return -1; // Unsupported scheme
    }
}


///////////  DEPRECATED FUNCTIONS  ///////////
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