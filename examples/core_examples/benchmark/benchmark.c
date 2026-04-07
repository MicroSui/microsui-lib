#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <time.h>

#include "MicroSui.h"

// Sui Message in hex format (this message must be signed)
const char* message_hex = "00000200080065cd1d0000000000202e3d52393c9035afd1ef38abd7fce2dad71f0e276b522fb274f4e14d1df974720202000101000001010300000000010100d79a4c7a655aa80cf92069bbac9666705f1d7181ff9c2d59efbc7e6ec4c3379d0180dc491e55e7caabfcdd1b0f538928d8d54107b9c1def3ed0baa3aa5106ba8674f0dd01400000000204b7e9da00f30cd1edf4d40710213c15a862e1fc175f2edb2b2c870c8559d65cdd79a4c7a655aa80cf92069bbac9666705f1d7181ff9c2d59efbc7e6ec4c3379de80300000000000040ab3c000000000000";

// You must place your seed private key (in bytes format) here.
const uint8_t private_key_seed[32] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

uint8_t random_digest[32] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
    0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F
};

uint64_t millis(void);

int main() {
    printf("\n\t\t\t --- MicroSui Operations Benchmarks ---\n");

    printf("\n\t This benchmark will measure the execution time of key operations in the MicroSui library, including signature generation and verification, as well as key management functions.\n");
    printf("\t The time taken for each operation will be displayed in milliseconds (ms) or microseconds (us) depending on the duration, providing insights into the performance of the library's core functionalities, and may vary significantly based on the underlying hardware and optimizations.\n\n");

    printf("\n\tOriginal Message: '%s'", message_hex);

    // 0. Converting the message from hex to bytes
    size_t message_len = strlen(message_hex) / 2; // 2 hex chars = 1 byte
    uint8_t message[message_len];
    hex_to_bytes(message_hex, message, message_len);

    // 0. Generate public and secret key
    uint8_t secret_key[64];
    uint8_t public_key[32];
    microsui_derive_keypair_ed25519(secret_key, public_key, private_key_seed);

    /////// SIGNATURES ///////
    // 1. Generating the Sui Signature from the message and seed private key
    printf("\n\n\n   1 - Generating Signature from Sui Message...\n");
    uint8_t sui_sig[97];

    uint64_t start_time = millis();
    microsui_sign_ed25519(sui_sig, message, message_len, private_key_seed);
    uint64_t end_time = millis();
    printf("\t Signature created successfully\n\n");

    // Calculate the time taken for verification
    uint64_t time_taken_signature = end_time - start_time;

    // 2. Generating the Sui Signature from the precalculated digest and secret key
    printf("   2 - Generating Signature from precalculated Digest...\n");
    uint8_t sui_sig_from_digest[97];

    start_time = millis();
    microsui_sign_ed25519_from_digest(sui_sig_from_digest, random_digest, secret_key, public_key);
    end_time = millis();

    uint64_t time_taken_signature_from_digest = end_time - start_time;
    printf("\t Signature from digest created successfully\n\n");

    // 3. Verifying the Sui Signature from the message and signature
    printf("   3 - Verifying Signature from Sui Message...\n");
    start_time = millis();
    int verification_result = microsui_verify_signature(sui_sig, message, message_len);
    end_time = millis();

    // Calculate the time taken for verification
    uint64_t time_taken_verification = end_time - start_time;
    if(verification_result == 0) {
        printf("\t Signature verified successfully\n\n");
    } else {
        printf("\t Signature verification failed --\n");
        printf("\t Error code: %d --\n\n", verification_result);
    }

    // 4. Verifying the Sui Signature from the precalculated digest and signature
    printf("   4 - Verifying Signature from precalculated Digest...\n");
    start_time = millis();
    int verification_result_from_digest = microsui_verify_signature_ed25519_from_digest(sui_sig_from_digest, random_digest);
    end_time = millis();

    // Calculate the time taken for verification
    uint64_t time_taken_verification_from_digest = end_time - start_time;
    if(verification_result_from_digest == 0) {
        printf("\t Signature from digest verified successfully\n\n");
    } else {
        printf("\t Signature verification failed --\n");
        printf("\t Error code: %d --\n\n", verification_result_from_digest);
    }

    /////// KEY MANAGEMENT ///////
    printf("   5 - Deriving a Public Key from a 32-byte Ed25519 seed private key...\n");
    uint8_t derived_public_key_out[32];
    start_time = millis();
    int public_key_derivation_result = microsui_derive_public_key_ed25519(derived_public_key_out, private_key_seed);
    end_time = millis();
    uint64_t time_taken_public_key_derivation = end_time - start_time;
    if(public_key_derivation_result == 0) {
        printf("\t Public key derived successfully.\n\n");
    } else {
        printf("\t Public key derivation failed --\n\n");
        printf("\t Error code: %d --\n\n", public_key_derivation_result);
    }

    printf("   6 - Deriving a Keypair (Secret Key + Public Key) from a 32-byte Ed25519 seed private key...\n");
    u_int8_t derived_secret_key_out[64];
    uint8_t derived_public_key_out2[32];
    start_time = millis();
    int keypair_derivation_result = microsui_derive_keypair_ed25519(derived_secret_key_out, derived_public_key_out2, private_key_seed);
    end_time = millis();
    uint64_t time_taken_keypair_derivation = end_time - start_time;
    if(keypair_derivation_result == 0) {
        printf("\t Keypair derived successfully.\n\n");
    } else {
        printf("\t Keypair derivation failed --\n\n");
        printf("\t Error code: %d --\n\n", keypair_derivation_result);
    }

    printf("   7 - Deriving a Sui address from a 32-byte Ed25519 public key...\n");
    uint8_t encoded_address[32];
    start_time = millis();
    int address_derivation_result = microsui_derive_sui_address_ed25519(encoded_address, public_key);
    end_time = millis();
    uint64_t time_taken_address_derivation = end_time - start_time;

    if(address_derivation_result == 0) {
        printf("\t Sui address derived successfully. Address: 0x");
        for (int i = 0; i < 32; i++) printf("%02x", encoded_address[i]);
        printf("\n\n");
    } else {
        printf("\t Sui address derivation failed --\n\n");
        printf("\t Error code: %d --\n\n", address_derivation_result);
    }


    /////// KEY MANAGEMENT - BECH32 OPERATIONS ///////
    printf("   8 - Encoding the 32-byte seed into a Sui Bech32 private key string....\n");
    char private_key_bech32_output[71];
    start_time = millis();
    int encoding_result = microsui_encode_bech32_private_key(private_key_bech32_output, random_digest);
    end_time = millis();

    // Calculate the time taken for encoding
    uint64_t time_taken_encoding = end_time - start_time;
    if(encoding_result == 0) {
        printf("\t Encoding successful. Encoded Bech32 Key: %s\n\n", private_key_bech32_output);
    } else {
        printf("\t Encoding failed --\n");
        printf("\t Error code: %d --\n\n", encoding_result);
    }

    printf("   9 - Decoding the Sui Bech32 private key string into a 32 raw seed bytes...\n");
    uint8_t private_key_output[32];
    start_time = millis();
    int decoding_result = microsui_decode_bech32_private_key(private_key_output, private_key_bech32_output);
    end_time = millis();

    // Calculate the time taken for decoding
    uint64_t time_taken_decoding = end_time - start_time;
    if(decoding_result == 0) {
        printf("\t Decoding successful.\n\n");
    } else {
        printf("\t Decoding failed --\n");
        printf("\t Error code: %d --\n\n", decoding_result);
    }



    printf("\t  MicroSui Benchmarks Results:\n");
    printf("\t      Signatures:\n");
    printf("\t\t Time taken for signing from Sui Message: ");
    if (time_taken_signature > 100000) {
        printf("\t\t %llu ms\n", (unsigned long long)(time_taken_signature / 1000));
    } else {
        printf("\t\t %.2f ms (%llu us) \n", (double)time_taken_signature / 1000.0, (unsigned long long)time_taken_signature);
    }

    printf("\t\t Time taken for signing from precalculated digest: ");
    if (time_taken_signature_from_digest > 100000) {
        printf("\t %llu ms\n", (unsigned long long)(time_taken_signature_from_digest / 1000));
    } else {
        printf("\t %.2f ms (%llu us) \n", (double)time_taken_signature_from_digest / 1000.0, (unsigned long long)time_taken_signature_from_digest);
    }

    printf("\n\t      Signature Check / Verification:\n");
    printf("\t\t Time taken for sign check from Sui Message: ");
    if (time_taken_verification > 100000) {
        printf("\t\t %llu ms\n", (unsigned long long)(time_taken_verification / 1000));
    } else {
        printf("\t\t %.2f ms (%llu us) \n", (double)time_taken_verification / 1000.0, (unsigned long long)time_taken_verification);
    }

    printf("\t\t Time taken for sign check from precalculated digest: ");
    if (time_taken_verification_from_digest > 100000) {
        printf("\t %llu ms\n", (unsigned long long)(time_taken_verification_from_digest / 1000));
    } else {
        printf("\t %.2f ms (%llu us) \n", (double)time_taken_verification_from_digest / 1000.0, (unsigned long long)time_taken_verification_from_digest);
    }

    printf("\n\t      Key Management Operations:\n");
    printf("\t\t Time taken for deriving public key from seed: ");
    if (time_taken_public_key_derivation > 100000) {
        printf("\t\t %llu ms\n", (unsigned long long)(time_taken_public_key_derivation / 1000));
    } else {
        printf("\t\t %.2f ms (%llu us) \n", (double)time_taken_public_key_derivation / 1000.0, (unsigned long long)time_taken_public_key_derivation);
    }

    printf("\t\t Time taken for deriving keypair from seed: ");
    if (time_taken_keypair_derivation > 100000) {
        printf("\t\t %llu ms\n", (unsigned long long)(time_taken_keypair_derivation / 1000));
    } else {
        printf("\t\t %.2f ms (%llu us) \n", (double)time_taken_keypair_derivation / 1000.0, (unsigned long long)time_taken_keypair_derivation);
    }

    printf("\t\t Time taken for deriving Sui address from public key: ");
    if (time_taken_address_derivation > 100000) {
        printf("\t %llu ms\n", (unsigned long long)(time_taken_address_derivation / 1000));
    } else {
        printf("\t %.2f ms (%llu us) \n", (double)time_taken_address_derivation / 1000.0, (unsigned long long)time_taken_address_derivation);
    }

    printf("\n\t      BECH32 Operations:\n");
    printf("\t\t Time taken for seed/private key bech32 encoding: ");
    if (time_taken_encoding > 100000) {
        printf("\t %llu ms\n", (unsigned long long)(time_taken_encoding / 1000));
    } else {
        printf("\t %.2f ms (%llu us) \n", (double)time_taken_encoding / 1000.0, (unsigned long long)time_taken_encoding);
    }
    printf("\t\t Time taken for seed/private key bech32 decoding: ");
    if (time_taken_decoding > 100000) {
        printf("\t %llu ms\n", (unsigned long long)(time_taken_decoding / 1000));
    } else {
        printf("\t %.2f ms (%llu us) \n", (double)time_taken_decoding / 1000.0, (unsigned long long)time_taken_decoding);
    }

    printf("\n \tEnd of benchmarks.\n");

    return 0;
}

uint64_t millis(void)
{
#if defined(_WIN32) || defined(_WIN64)
    // Windows: QueryPerformanceCounter (high-resolution timer)
    static LARGE_INTEGER freq = {0};
    LARGE_INTEGER now;

    if (freq.QuadPart == 0)
        QueryPerformanceFrequency(&freq);

    QueryPerformanceCounter(&now);
    return (uint64_t)((now.QuadPart * 1000000) / freq.QuadPart);

#else
    // Linux y macOS: clock_gettime(CLOCK_MONOTONIC)
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)(ts.tv_sec) + (ts.tv_nsec / 1000ULL);
#endif
}