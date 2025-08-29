// jsmn_helpers.c
#include <string.h>
#define JSMN_HEADER
#include "jsmn.h"

int jsmn_jsoneq(const char *json, const jsmntok_t *tok, const char *s) {
    if (!tok || tok->type != JSMN_STRING) return -1;
    size_t tlen = (size_t)(tok->end - tok->start);
    size_t slen = strlen(s);
    return (tlen == slen && strncmp(json + tok->start, s, tlen) == 0) ? 0 : -1;
}

void jsmn_tok_copy_cstr(const char *json, const jsmntok_t *tok,
                        char *out, size_t out_sz) {
    if (!out || out_sz == 0 || !tok) return;
    size_t len = (size_t)(tok->end - tok->start);
    if (len >= out_sz) len = out_sz - 1;
    memcpy(out, json + tok->start, len);
    out[len] = '\0';
}

int jsmn_skip_token(const jsmntok_t *toks, int idx) {
    switch (toks[idx].type) {
        case JSMN_PRIMITIVE:
        case JSMN_STRING:
            return idx + 1;

        case JSMN_ARRAY: {
            int n = toks[idx].size;
            idx += 1;
            for (int i = 0; i < n; i++) idx = jsmn_skip_token(toks, idx);
            return idx;
        }

        case JSMN_OBJECT: {
            int n = toks[idx].size; // number of key-value pairs
            idx += 1;
            for (int i = 0; i < n; i++) {
                idx += 1;                    // key (STRING)
                idx = jsmn_skip_token(toks, idx); // value subtree
            }
            return idx;
        }

        default:
            return idx + 1;
    }
}

int jsmn_object_get_value_idx(const char *json, const jsmntok_t *toks, int tokc,
                              int obj_idx, const char *key, int *out_val_idx) {
    if (!json || !toks || !out_val_idx) return -1;
    if (obj_idx < 0 || obj_idx >= tokc || toks[obj_idx].type != JSMN_OBJECT) return -2;

    int pairs = toks[obj_idx].size;
    int idx = obj_idx + 1; // first key
    for (int i = 0; i < pairs; i++) {
        if (idx >= tokc) return -3;
        if (toks[idx].type != JSMN_STRING) return -4;
        if (jsmn_jsoneq(json, &toks[idx], key) == 0) {
            if (idx + 1 >= tokc) return -5;
            *out_val_idx = idx + 1;
            return 0;
        }
        idx = jsmn_skip_token(toks, idx + 1); // jump over value
    }
    return -6; // not found
}

int jsmn_find_array_by_key_anywhere(const char *json, const jsmntok_t *toks, int tokc,
                                    const char *key) {
    if (!json || !toks || !key) return -1;
    for (int i = 1; i + 1 < tokc; i++) {
        if (toks[i].type == JSMN_STRING && jsmn_jsoneq(json, &toks[i], key) == 0) {
            int val_idx = i + 1;
            if (val_idx < tokc && toks[val_idx].type == JSMN_ARRAY) return val_idx;
        }
    }
    return -2;
}

int jsmn_array_nth_child_idx(const jsmntok_t *toks, int tokc, int arr_idx, int index) {
    if (!toks) return -1;
    if (arr_idx < 0 || arr_idx >= tokc || toks[arr_idx].type != JSMN_ARRAY) return -2;
    if (index < 0 || index >= toks[arr_idx].size) return -3;

    int idx = arr_idx + 1; // first element
    for (int i = 0; i < index; i++) {
        if (idx >= tokc) return -4;
        idx = jsmn_skip_token(toks, idx);
    }
    if (idx >= tokc) return -5;
    return idx;
}