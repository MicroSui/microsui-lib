#ifndef RPC_JSON_BUILDER_H
#define RPC_JSON_BUILDER_H

#include <stdint.h>
#include <stddef.h>

char* microsui_prepare_executeTransactionBlock(const uint8_t sui_sig[97], const uint8_t* sui_msg, size_t sui_msg_len);

#endif