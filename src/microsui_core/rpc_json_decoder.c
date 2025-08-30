// json_balance_changes.c
// Public functions specific to result.balanceChanges using jsmn + helpers.
// - json_get_balanceChanges_len(json)
// - json_get_balanceChanges_amount_at(json, index, out, out_sz)
//
// Returns 0 on success (for _amount_at) or a non-negative length (for _len).
// Negative values are error codes.

#include <string.h>
#include "lib/jsmn/jsmn_helpers.h"
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

// ---------- Internal structs----------
// Reusable context after parsing once
typedef struct {
    const char   *json;       // Original JSON string (must remain alive)
    jsmntok_t    *toks;       // Token buffer provided by caller
    int           tokc;       // Number of valid tokens
    jsmntype_t    root_type;  // Type of root token (OBJECT/ARRAY/...)
} jsmn_ctx;

// ---------- Internal helpers (arena) ----------
static void arena_reset(SuiTransactionBlockResponse* r) {
    r->_used = 0;
}

static char* arena_copy_token(SuiTransactionBlockResponse* r,
                              const char* json, const jsmntok_t* tok) {
    size_t len = (size_t)(tok->end - tok->start);
    if (r->_used + len + 1 > RESP_ARENA_SIZE) return NULL;
    char* dst = r->_arena + r->_used;
    memcpy(dst, json + tok->start, len);
    dst[len] = '\0';
    r->_used += (len + 1);
    return dst;
}

// Copies a C string literal into the arena.
static char* arena_copy_cstr(SuiTransactionBlockResponse* r, const char* s) {
    size_t len = strlen(s);
    if (r->_used + len + 1 > RESP_ARENA_SIZE) return NULL;
    char* dst = r->_arena + r->_used;
    memcpy(dst, s, len + 1);
    r->_used += (len + 1);
    return dst;
}

int jsmn_parse_all(const char *json, jsmntok_t *toks, int max_toks, jsmn_ctx *out_ctx) {
    if (!json || !toks || max_toks <= 0 || !out_ctx) return -1;

    jsmn_parser p;
    jsmn_init(&p);

    int tokc = jsmn_parse(&p, json, (int)strlen(json), toks, max_toks);
    if (tokc < 0) {
        // jsmn: JSMN_ERROR_NOMEM (-1), JSMN_ERROR_INVAL (-2), JSMN_ERROR_PART (-3)
        return tokc;
    }
    if (tokc == 0) return -4; // Empty JSON or whitespace only

    out_ctx->json      = json;
    out_ctx->toks      = toks;
    out_ctx->tokc      = tokc;
    out_ctx->root_type = toks[0].type;
    return 0;
}


// Find a top-level (anywhere) key and return its value token index.
static int find_key_value_anywhere(const char* json, const jsmntok_t* toks, int tokc, const char* key) {
    for (int i = 1; i + 1 < tokc; i++) {
        if (toks[i].type == JSMN_STRING && jsmn_jsoneq(json, &toks[i], key) == 0) {
            return i + 1;
        }
    }
    return -1;
}

static char* extract_owner_string(SuiTransactionBlockResponse* out, const char* json, const jsmntok_t* toks, int tokc, int owner_idx) {
    if (owner_idx < 0 || owner_idx >= tokc) return NULL;
    const jsmntok_t* t = &toks[owner_idx];

    if (t->type == JSMN_OBJECT) {
        int addr_idx = -1;
        if (jsmn_object_get_value_idx(json, toks, tokc, owner_idx, "AddressOwner", &addr_idx) == 0) {
            return arena_copy_token(out, json, &toks[addr_idx]);
        }
        // fallback: not AddressOwner -> just return a tag
        return arena_copy_cstr(out, "[owner:object]");
    } else if (t->type == JSMN_STRING || t->type == JSMN_PRIMITIVE) {
        return arena_copy_token(out, json, t);
    } else {
        return arena_copy_cstr(out, "[owner:unsupported]");
    }
}

static char* copy_key_anywhere(SuiTransactionBlockResponse* out, const jsmn_ctx* ctx, const char* key) {
    int v = find_key_value_anywhere(ctx->json, ctx->toks, ctx->tokc, key);
    if (v < 0) return NULL;
    // Copy primitive/string as text. If it's object/array, store a tag.
    if (ctx->toks[v].type == JSMN_STRING || ctx->toks[v].type == JSMN_PRIMITIVE) {
        return arena_copy_token(out, ctx->json, &ctx->toks[v]);
    }
    return arena_copy_cstr(out, "[unsupported]");
}

// Prefer result.digest if available; fall back to the first "digest" found anywhere.
static char* copy_digest(SuiTransactionBlockResponse* out, const jsmn_ctx* ctx) {
    // Try result.digest
    int result_idx = find_key_value_anywhere(ctx->json, ctx->toks, ctx->tokc, "result");
    if (result_idx >= 0 && ctx->toks[result_idx].type == JSMN_OBJECT) {
        int d_idx = -1;
        if (jsmn_object_get_value_idx(ctx->json, ctx->toks, ctx->tokc, result_idx, "digest", &d_idx) == 0) {
            if (ctx->toks[d_idx].type == JSMN_STRING || ctx->toks[d_idx].type == JSMN_PRIMITIVE) {
                return arena_copy_token(out, ctx->json, &ctx->toks[d_idx]);
            }
        }
    }
    // Fallback: first "digest" anywhere
    return copy_key_anywhere(out, ctx, "digest");
}

static void fill_balance_changes(SuiTransactionBlockResponse* out, const jsmn_ctx* ctx) {
    // Locate the balanceChanges array anywhere (typically under "result").
    int arr_idx = jsmn_find_array_by_key_anywhere(ctx->json, ctx->toks, ctx->tokc, "balanceChanges");
    if (arr_idx < 0) {
        out->balanceChanges_len = 0;
        return;
    }

    int want = ctx->toks[arr_idx].size;
    if (want > MAX_BALANCE_CHANGES) want = MAX_BALANCE_CHANGES;
    out->balanceChanges_len = want;

    int idx = arr_idx + 1; // first element token
    for (int i = 0; i < out->balanceChanges_len; i++) {
        if (idx >= ctx->tokc) { out->balanceChanges_len = i; break; }
        const jsmntok_t* elem = &ctx->toks[idx];
        if (elem->type != JSMN_OBJECT) {
            // skip malformed element
            idx = jsmn_skip_token(ctx->toks, idx);
            continue;
        }

        // Defaults to NULL
        out->balanceChanges[i].amount   = NULL;
        out->balanceChanges[i].coinType = NULL;
        out->balanceChanges[i].owner    = NULL;

        // amount
        int amount_idx = -1;
        if (jsmn_object_get_value_idx(ctx->json, ctx->toks, ctx->tokc, idx, "amount", &amount_idx) == 0) {
            out->balanceChanges[i].amount = arena_copy_token(out, ctx->json, &ctx->toks[amount_idx]);
        }
        // coinType
        int coin_idx = -1;
        if (jsmn_object_get_value_idx(ctx->json, ctx->toks, ctx->tokc, idx, "coinType", &coin_idx) == 0) {
            out->balanceChanges[i].coinType = arena_copy_token(out, ctx->json, &ctx->toks[coin_idx]);
        }
        // owner
        int owner_idx = -1;
        if (jsmn_object_get_value_idx(ctx->json, ctx->toks, ctx->tokc, idx, "owner", &owner_idx) == 0) {
            out->balanceChanges[i].owner = extract_owner_string(out, ctx->json, ctx->toks, ctx->tokc, owner_idx);
        }

        // advance to next array element
        idx = jsmn_skip_token(ctx->toks, idx);
    }
}

/**
 * @brief Parse a JSON string into a SuiTransactionBlockResponse structure.
 *
 * Uses the JSMN JSON parser to tokenize the input string and extract
 * relevant fields of a transaction block response. Populates the provided
 * output struct with parsed data such as balance changes, checkpoint,
 * confirmedLocalExecution, and digest.
 *
 * @param[in]  json   Null-terminated JSON string containing the transaction block response.
 * @param[out] out    Pointer to an initialized SuiTransactionBlockResponse struct.
 *
 * @return 0 on success; non-zero error code if parsing failed.
 *
 * @note A static token buffer is used internally to reduce stack usage.
 * @note On parse error, the `digest` field in `out` is set to "[parse_error]".
 * @note The output struct must be arena-managed; helper functions like
 *       `arena_reset()` and `arena_copy_cstr()` are used internally.
 */
int microsui_generate_tx_block_response_from_json(const char* json, SuiTransactionBlockResponse* out) {
    static jsmntok_t tokens[JSMN_MAX_TOKENS];
    jsmn_ctx ctx;

    if (!out) return -1;
    arena_reset(out);
    out->balanceChanges_len      = 0;
    out->checkpoint              = NULL;
    out->confirmedLocalExecution = NULL;
    out->digest                  = NULL;

    int rc = jsmn_parse_all(json, tokens, JSMN_MAX_TOKENS, &ctx);
    if (rc != 0) {
        out->digest = arena_copy_cstr(out, "[parse_error]");
        return rc;
    }

    fill_balance_changes(out, &ctx);
    out->checkpoint              = copy_key_anywhere(out, &ctx, "checkpoint");
    out->confirmedLocalExecution = copy_key_anywhere(out, &ctx, "confirmedLocalExecution");
    out->digest                  = copy_digest(out, &ctx);
    return 0;
}