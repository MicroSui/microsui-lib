#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "microsui/byte_conversions.h"
#include "microsui/rpc_json_builder.h"
#include "microsui/http_router.h"

// Sui Message and Signature in hex format
const char* sui_message = "0000020008c0320a030000000000202e3d52393c9035afd1ef38abd7fce2dad71f0e276b522fb274f4e14d1df974720202000101000001010300000000010100d79a4c7a655aa80cf92069bbac9666705f1d7181ff9c2d59efbc7e6ec4c3379d0115bc7e3113dfaebc7bdb30676e56e6ff651365235b74a202cbb1e73f57eaeb78600fd0140000000020c00463b22a32bebcb028b264d61bfa963c3f6a82bd7487c52bec8b2bf0c0e373d79a4c7a655aa80cf92069bbac9666705f1d7181ff9c2d59efbc7e6ec4c3379de80300000000000040ab3c000000000000";
const char* sui_signature = "004131446a8541b3c408e18054f2e175f1b04a93f6ab1bbf38d1234a3b6ed65adf508e9a03555269222536e0ecdbf4d496de7212c5e2f9c4dec2251226b7fcf8036309ede0a480229c12b578364ca13bc36cbf61786c47aa75f323127462b32405";

// RPC NODE
const char* host = "fullnode.testnet.sui.io";
const char* path = "/";
int port = 443;


int main() {
    printf("\n\t\t Prepare JSON and send it to Sui Transaction to the Sui Network (via Post HTTP in API RPC) example:\n");
    printf("\t This example will prepare a JSON string to send a transaction to the Sui Network.\n");
    printf("\t The JSON will be prepared using the Sui Signature and the Sui Message.\n");
    printf("\t After the JSON creation it will be sent to the Sui Network via HTTP POST request.\n");
    printf("\t Documentation: https://docs.sui.io/sui-api-ref#sui_executetransactionblock\n\n");

    // Convert the Sui Signature from hex to bytes
    uint8_t signature_bytes[132];
    hex_to_bytes(sui_signature, signature_bytes, sizeof(signature_bytes));

    // Convert the Sui Message from hex to bytes
    size_t message_bytes_len = strlen(sui_message) / 2; // 2 hex chars = 1 byte
    uint8_t message_bytes[message_bytes_len];
    hex_to_bytes(sui_message, message_bytes, message_bytes_len);

    // Prepare the JSON string for executing the transaction block
    char* json = microsui_prepare_executeTransactionBlock(signature_bytes, message_bytes, message_bytes_len);
    if (!json) {
        printf("Failed to generate JSON.\n");
        return -1;
    }
    printf("\t JSON prepared to send via RPC by method `sui_executetransactionblock`:\n %s\n\n", json);

    // Send the JSON via HTTP POST request
    printf("\t Sending HTTP POST request to %s:%d%s\n", host, port, path);
    char* response = microsui_http_post(host, path, port, json);

    if (response != NULL) {
        printf("HTTP POST success!\n");
        printf("\n\t Sui Network RPC Response:\n%s\n", response);
    } else {
        printf("\n HTTP POST failed\n");
    }

    free(json); // Free the JSON string after use (very important to avoid memory leaks)
    free(response); // Free the response string after use (very important to avoid memory leaks)

    return 0;
}