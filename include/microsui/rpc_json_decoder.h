#ifndef RPC_JSON_DECODER_H
#define RPC_JSON_DECODER_H

#include <stdint.h>
#include <stddef.h>
#define JSMN_HEADER
#include "lib/jsmn/jsmn.h"


// ---------- Tuning limits ----------
#ifndef MAX_BALANCE_CHANGES
#define MAX_BALANCE_CHANGES  16
#endif

#ifndef RESP_ARENA_SIZE
#define RESP_ARENA_SIZE      2048  // total bytes for all strings
#endif

#ifndef JSMN_MAX_TOKENS
#define JSMN_MAX_TOKENS      4000
#endif

// ---------- Public structs ----------

#ifndef RPC_RESPONSE_STRUCTS
#define RPC_RESPONSE_STRUCTS

typedef struct BalanceChange {
    char* amount;    // e.g., "100000000"
    char* coinType;  // e.g., "0x2::sui::SUI"
    char* owner;     // e.g., "0xabc..." (flattened AddressOwner if present)
} BalanceChange;

typedef struct SuiTransactionBlockResponse {
    BalanceChange balanceChanges[MAX_BALANCE_CHANGES];
    int           balanceChanges_len;       // actual count

    char* checkpoint;               // may be NULL if not present
    char* confirmedLocalExecution;  // "true"/"false" or NULL
    char* digest;                   // may be NULL if not present

    // Internal arena for string storage (do not use directly from outside)
    char   _arena[RESP_ARENA_SIZE];
    size_t _used;
} SuiTransactionBlockResponse;

#endif


// Reusable context after parsing once.
typedef struct {
    const char   *json;       // Original JSON string (must remain alive)
    jsmntok_t    *toks;       // Token buffer provided by caller
    int           tokc;       // Number of valid tokens
    jsmntype_t    root_type;  // Type of root token (OBJECT/ARRAY/...)
} jsmn_ctx;

int microsui_generate_tx_block_response_from_json(const char* json, SuiTransactionBlockResponse* out);

#endif