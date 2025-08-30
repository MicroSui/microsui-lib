#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "microsui/rpc_json_decoder.h"

int main() {
    printf("\t\t Generate Sui Treansaction Block Response Object from RPC JSON Response\n\n");

    const char *json_res = "{\"jsonrpc\":\"2.0\",\"id\":1,\"result\":{\"digest\":\"3KGok5sfir2w81rsZJjS67yqrutnQzYQuAJSLL11U41d\",\"checkpoint\":\"234188931\",\"effects\":{\"messageVersion\":\"v1\",\"status\":{\"status\":\"success\"},\"executedEpoch\":\"838\",\"gasUsed\":{\"computationCost\":\"1000000\",\"storageCost\":\"1976000\",\"storageRebate\":\"978120\",\"nonRefundableStorageFee\":\"9880\"},\"modifiedAtVersions\":[{\"objectId\":\"0x2901f275336715aa2d266fcdc57b3733fca4b135637160d6364acf4b7874590d\",\"sequenceNumber\":\"349178637\"}],\"transactionDigest\":\"3KGok5sfir2w81rsZJjS67yqrutnQzYQuAJSLL11U41d\",\"created\":[{\"owner\":{\"AddressOwner\":\"0x2e3d52393c9035afd1ef38abd7fce2dad71f0e276b522fb274f4e14d1df97472\"},\"reference\":{\"objectId\":\"0x9338c47ce756890f41e657bd628b133189eaf1da4be4923ba42384612bf406a8\",\"version\":349178638,\"digest\":\"HX6xumGqVqrZMGhWRCZUZJqe1Ffnzri1D6aUr93J4737\"}}],\"mutated\":[{\"owner\":{\"AddressOwner\":\"0x7a1378aafadef8ce743b72e8b248295c8f61c102c94040161146ea4d51a182b6\"},\"reference\":{\"objectId\":\"0x2901f275336715aa2d266fcdc57b3733fca4b135637160d6364acf4b7874590d\",\"version\":349178638,\"digest\":\"5rbDoKMTHVJpnmok9xh5kVhZgLQw4nekEMkEMHJGkPpi\"}}],\"gasObject\":{\"owner\":{\"AddressOwner\":\"0x7a1378aafadef8ce743b72e8b248295c8f61c102c94040161146ea4d51a182b6\"},\"reference\":{\"objectId\":\"0x2901f275336715aa2d266fcdc57b3733fca4b135637160d6364acf4b7874590d\",\"version\":349178638,\"digest\":\"5rbDoKMTHVJpnmok9xh5kVhZgLQw4nekEMkEMHJGkPpi\"}},\"dependencies\":[\"6uXQdvpSqtDgnKAcjZAKNfGgJUHYNGkpmPWEujGuUCmd\"]},\"events\":[],\"objectChanges\":[{\"type\":\"mutated\",\"sender\":\"0x7a1378aafadef8ce743b72e8b248295c8f61c102c94040161146ea4d51a182b6\",\"owner\":{\"AddressOwner\":\"0x7a1378aafadef8ce743b72e8b248295c8f61c102c94040161146ea4d51a182b6\"},\"objectType\":\"0x2::coin::Coin<0x2::sui::SUI>\",\"objectId\":\"0x2901f275336715aa2d266fcdc57b3733fca4b135637160d6364acf4b7874590d\",\"version\":\"349178638\",\"previousVersion\":\"349178637\",\"digest\":\"5rbDoKMTHVJpnmok9xh5kVhZgLQw4nekEMkEMHJGkPpi\"},{\"type\":\"created\",\"sender\":\"0x7a1378aafadef8ce743b72e8b248295c8f61c102c94040161146ea4d51a182b6\",\"owner\":{\"AddressOwner\":\"0x2e3d52393c9035afd1ef38abd7fce2dad71f0e276b522fb274f4e14d1df97472\"},\"objectType\":\"0x2::coin::Coin<0x2::sui::SUI>\",\"objectId\":\"0x9338c47ce756890f41e657bd628b133189eaf1da4be4923ba42384612bf406a8\",\"version\":\"349178638\",\"digest\":\"HX6xumGqVqrZMGhWRCZUZJqe1Ffnzri1D6aUr93J4737\"}],\"balanceChanges\":[{\"owner\":{\"AddressOwner\":\"0x2e3d52393c9035afd1ef38abd7fce2dad71f0e276b522fb274f4e14d1df97472\"},\"coinType\":\"0x2::sui::SUI\",\"amount\":\"170000000\"},{\"owner\":{\"AddressOwner\":\"0x7a1378aafadef8ce743b72e8b248295c8f61c102c94040161146ea4d51a182b6\"},\"coinType\":\"0x2::sui::SUI\",\"amount\":\"-101997880\"}],\"confirmedLocalExecution\":true}}";

    SuiTransactionBlockResponse res;

    microsui_generate_tx_block_response_from_json(json_res, &res);
    printf("\t\t\t Tx Response Analysis:\n");
    printf("\t Tx Digest= %s\n", res.digest);
    printf("\t Tx Checkpoint= %s\n", res.checkpoint);
    printf("\t Tx confirmedLocalExecution= %s\n", res.confirmedLocalExecution);

    printf("\n\t\t Balance Changes: %d\n", res.balanceChanges_len);
    for (int i = 0; i < res.balanceChanges_len; i++)
    {
        printf("\t Balances Changes Coin %d:\n", i+1);
        printf("\t\t balanceChanges[%d].amount= %s\n", i, res.balanceChanges[i].amount);
        printf("\t\t balanceChanges[%d].coinType= %s\n", i, res.balanceChanges[i].coinType);
        printf("\t\t balanceChanges[%d].owner= %s\n\n", i, res.balanceChanges[i].owner);
    }
}