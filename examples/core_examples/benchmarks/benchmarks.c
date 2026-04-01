#include <stdio.h>
#include <stdint.h>
#include <string.h>
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

uint64_t millis(void);

int main() {
    printf("\n\t\t\t --- MicroSui Operations Benchmarks ---\n");
    printf("\n\tOriginal Message: %s", message_hex);

    // Converting the message from hex to bytes
    size_t message_len = strlen(message_hex) / 2; // 2 hex chars = 1 byte
    uint8_t message[message_len];
    hex_to_bytes(message_hex, message, message_len);

    // Generating the Sui Signature from the message and private key
    uint8_t sui_sig[97];
    printf("\n\n\n  Generating Signature...\n");

    uint64_t start_time = millis();
    microsui_sign_ed25519(sui_sig, message, message_len, private_key_seed);
    uint64_t end_time = millis();
    printf("\t Signature created successfully\n");

    // Calculate the time taken for verification
    uint64_t time_taken_signature = end_time - start_time;

    // Verifying the Sui Signature
    start_time = millis();
    int verification_result = microsui_verify_signature(sui_sig, message, message_len);
    end_time = millis();

    // Calculate the time taken for verification
    uint64_t time_taken_verification = end_time - start_time;
    if(verification_result == 0) {
        printf("\t Signature verified successfully\n\n");
    } else {
        printf("\t Signature verification failed\n");
        printf("\t Error code: %d\n\n", verification_result);
    }


    printf("\t Benchmarks Results:\n");
    printf("\t\t Time taken for signing: ");
    if (time_taken_signature > 100000) {
        printf("%llu ms\n", (unsigned long long)(time_taken_signature / 1000));
    } else {
        printf("%.2f ms (%llu us) \n", (double)time_taken_signature / 1000.0, (unsigned long long)time_taken_signature);
    }

    printf("\t\t Time taken for verification: ");
    if (time_taken_verification > 100000) {
        printf("%llu ms\n", (unsigned long long)(time_taken_verification / 1000));
    } else {
        printf("%.2f ms (%llu us) \n", (double)time_taken_verification / 1000.0, (unsigned long long)time_taken_verification);
    }
    printf("\n End of benchmarks.\n");

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