#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "cryptography.h"


static const char ALPHABET[] = "qpzry9x8gf2tvdw0s3jn54khce6mua7l";

static const int8_t ALPHABET_MAP[128] = {
    ['q'] = 0,  ['p'] = 1,  ['z'] = 2,  ['r'] = 3,
    ['y'] = 4,  ['9'] = 5,  ['x'] = 6,  ['8'] = 7,
    ['g'] = 8,  ['f'] = 9,  ['2'] = 10, ['t'] = 11,
    ['v'] = 12, ['d'] = 13, ['w'] = 14, ['0'] = 15,
    ['s'] = 16, ['3'] = 17, ['j'] = 18, ['n'] = 19,
    ['5'] = 20, ['4'] = 21, ['k'] = 22, ['h'] = 23,
    ['c'] = 24, ['e'] = 25, ['6'] = 26, ['m'] = 27,
    ['u'] = 28, ['a'] = 29, ['7'] = 30, ['l'] = 31
};

#define TOLOWER(c)  ( ((unsigned char)(c) >= 'A' && (unsigned char)(c) <= 'Z') \
                          ? ((unsigned char)(c) + ('a' - 'A'))                   \
                          : (unsigned char)(c) )

static uint32_t bech32_polymod(const uint8_t *values, size_t values_len) {
    uint32_t c = 1;
    static const uint32_t GEN[5] = {
        0x3b6a57b2UL,
        0x26508e6dUL,
        0x1ea119faUL,
        0x3d4233ddUL,
        0x2a1462b3UL
    };
    for (size_t i = 0; i < values_len; i++) {
        uint8_t v = values[i];
        uint8_t c0 = c >> 25;
        c = ((c & 0x1ffffff) << 5) ^ v;
        for (int j = 0; j < 5; j++) {
            if ((c0 >> j) & 1) {
                c ^= GEN[j];
            }
        }
    }
    return c;
}

static bool convert_bits(uint8_t *out, size_t *out_len, const uint8_t *in, size_t in_len, int from_bits, int to_bits, bool pad) {
    uint32_t acc = 0;
    int bits = 0;
    size_t out_index = 0;
    uint32_t maxv = (1U << to_bits) - 1;
    uint32_t max_acc = (1U << (from_bits + to_bits - 1)) - 1;
    for (size_t i = 0; i < in_len; i++) {
        acc = ((acc << from_bits) | in[i]) & max_acc;
        bits += from_bits;
        while (bits >= to_bits) {
            bits -= to_bits;
            if (out) {
                out[out_index] = (acc >> bits) & maxv;
            }
            out_index++;
        }
    }
    if (pad && bits > 0) {
        if (out) {
            out[out_index] = (acc << (to_bits - bits)) & maxv;
        }
        out_index++;
    }
    *out_len = out_index;
    return true;
}

static void expand_hrp(const char *hrp, uint8_t *hrp_expanded, size_t *hrp_exp_len) {
    size_t hrp_len = strlen(hrp);
    for (size_t i = 0; i < hrp_len; i++) {
        hrp_expanded[i] = hrp[i] >> 5;
    }
    hrp_expanded[hrp_len] = 0;
    for (size_t i = 0; i < hrp_len; i++) {
        hrp_expanded[hrp_len + 1 + i] = hrp[i] & 0x1F;
    }
    *hrp_exp_len = hrp_len * 2 + 1;
}

static void bech32_create_checksum(const char *hrp, const uint8_t *data, size_t data_len, uint8_t *checksum) {
    size_t hrp_exp_len = 0;
    uint8_t *values = malloc((strlen(hrp) * 2 + 1 + data_len + 6) * sizeof(uint8_t));
    expand_hrp(hrp, values, &hrp_exp_len);
    memcpy(values + hrp_exp_len, data, data_len);

    for (int i = 0; i < 6; i++) {
        values[hrp_exp_len + data_len + i] = 0;
    }
    uint32_t mod = bech32_polymod(values, hrp_exp_len + data_len + 6) ^ 1;
    free(values);

    for (int i = 0; i < 6; i++) {
        checksum[i] = (mod >> (5 * (5 - i))) & 0x1F;
    }
}

/**
 * @brief Decode a Sui Bech32 private key string into 32 raw bytes.
 *
 * Validates the Bech32-encoded private key (HRP = "suiprivkey", no mixed case),
 * converts the 5-bit words back to 8-bit bytes, and extracts the 32-byte secret
 * (sk) after the 1-byte scheme flag.
 *
 * @param[in]  privkey_bech          Null-terminated Bech32 string ("suiprivkey1...").
 * @param[out] privkey_bytes_output  Output buffer for the 32-byte secret key.
 *
 * @return 0 on success; -1 on invalid length/format, mixed case, bad alphabet,
 *         conversion failure, or unexpected payload size.
 *
 * @note Expects total string length == PK_BECH32_LEN. Mixed case is rejected.
 * @note The returned key is the 32-byte seed; callers must handle it securely.
 */
int microsui_decode_sui_privkey(const char *privkey_bech, uint8_t privkey_bytes_output[32]) {
    size_t len = strlen(privkey_bech);
    if (len != PK_BECH32_LEN) return -1;

    bool hasLower = false, hasUpper = false;
    for (size_t i = 0; i < len; i++) {
        char c = privkey_bech[i];
        if (c >= 'a' && c <= 'z') hasLower = true;
        if (c >= 'A' && c <= 'Z') hasUpper = true;
    }
    if (hasLower && hasUpper) return -1;

    char str[PK_BECH32_LEN + 1];
    for (size_t i = 0; i < len; i++) {
        str[i] = TOLOWER(privkey_bech[i]);
    }
    str[len] = '\0';

    char *pos = strrchr(str, '1');
    if (!pos) return -1;
    size_t sep = pos - str;
    if (sep < 1 || sep + 7 > len) return -1;

    if (strncmp(str, "suiprivkey", 10) != 0) return -1;

    size_t data_len = len - sep - 1;
    uint8_t data5[PK_BECH32_LEN + 1];
    for (size_t i = 0; i < data_len; i++) {
        char c = str[sep + 1 + i];
        if (c < 0 || c >= 127 || ALPHABET_MAP[(int)c] == -1) return -1;
        data5[i] = (uint8_t)ALPHABET_MAP[(int)c];
    }

    if (data_len < 6) return -1;
    size_t words_len = data_len - 6;
    uint8_t words[PK_BECH32_LEN + 1];
    memcpy(words, data5, words_len);

    const char *hrp = str;
    size_t hrp_len = sep;
    uint8_t hrp_expanded[PK_BECH32_LEN + 1];
    size_t hrp_exp_len = 0;
    for (size_t i = 0; i < hrp_len; i++) hrp_expanded[hrp_exp_len++] = (uint8_t)(hrp[i] >> 5);
    hrp_expanded[hrp_exp_len++] = 0;
    for (size_t i = 0; i < hrp_len; i++) hrp_expanded[hrp_exp_len++] = (uint8_t)(hrp[i] & 0x1F);

    uint8_t ext_secret[35];
    size_t ext_len = 0;
    if (!convert_bits(ext_secret, &ext_len, words, words_len, 5, 8, false)) return -1;
    if (ext_len != 33) return -1;

    memcpy(privkey_bytes_output, ext_secret + 1, 32);
    return 0;
}

/**
 * @brief Encode a 32-byte Sui private key into a Bech32 string.
 *
 * Builds the Bech32 payload as: [1-byte scheme flag (0x00) | 32-byte secret],
 * converts to 5-bit words, appends checksum, and writes the Bech32 string with
 * HRP = "suiprivkey".
 *
 * @param[in]  privkey_bytes        Pointer to the 32-byte secret key.
 * @param[out] privkey_bech_output  Output buffer for the null-terminated Bech32 string.
 *                                  Must have capacity >= PK_BECH32_LEN + 1.
 *
 * @return 0 on success; -1 on conversion failure or insufficient output capacity.
 *
 * @note The output uses lowercase Bech32 alphabet and includes the null terminator.
 * @note Callers are responsible for providing a sufficiently large output buffer.
 */
int microsui_encode_sui_privkey(const uint8_t *privkey_bytes, char *privkey_bech_output) {
    const char *hrp = "suiprivkey";
    const uint8_t scheme_flag = 0x00;
    const size_t output_len = PK_BECH32_LEN + 1;

    uint8_t data[33];
    data[0] = scheme_flag;
    memcpy(data + 1, privkey_bytes, 32);

    size_t data5_len = 0;
    uint8_t data5[(33 * 8 + 4) / 5 + 1];
    if (!convert_bits(data5, &data5_len, data, 33, 8, 5, true)) {
        return -1;
    }

    uint8_t checksum[6];
    bech32_create_checksum(hrp, data5, data5_len, checksum);

    size_t hrp_len = strlen(hrp);
    if (hrp_len + 1 + data5_len + 6 + 1 > output_len) {
        return -1;
    }

    for (size_t i = 0; i < hrp_len; i++) {
        privkey_bech_output[i] = hrp[i];
    }
    privkey_bech_output[hrp_len] = '1';

    size_t idx = hrp_len + 1;
    for (size_t i = 0; i < data5_len; i++) {
        privkey_bech_output[idx++] = ALPHABET[data5[i]];
    }

    for (int i = 0; i < 6; i++) {
        privkey_bech_output[idx++] = ALPHABET[checksum[i]];
    }
    privkey_bech_output[idx] = '\0'; // null-terminator
    return 0;
}