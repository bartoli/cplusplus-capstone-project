// Capstone program entry point.
//
// Drive the Bitcoin Core regtest node through the full workflow and write the
// 10-line out.txt at the repo root.
//
// Workflow to implement (see README.md for details):
//   1. Create/load the "Miner" and "Trader" wallets (idempotent).
//   2. Generate a Miner address (label "Mining Reward") and mine 101 blocks to
//      it so the first coinbase reward matures (100-confirmation rule).
//   3. Print the Miner balance.
//   4. Generate a Trader receiving address (label "Received").
//   5. Send 20 BTC from Miner to Trader.
//   6. Confirm the transaction is in the mempool.
//   7. Mine 1 block to confirm it.
//   8. Fetch transaction details (gettransaction / getrawtransaction /
//      decoderawtransaction) and write the report to out.txt.

#include "main.h"
#include "rpc_client.h"

#include <curl/curl.h>

#include <iostream>

int main() {
    curl_global_init(CURL_GLOBAL_DEFAULT);

    int exit_code = 0;
    try {
        RpcConfig cfg;  // defaults: alice:password @ 127.0.0.1:18443
        BitcoinRPC client(cfg);

        // TODO: implement the capstone workflow described above and write out.txt.
        (void)client;
        std::cerr << "Not implemented yet.\n";
        exit_code = 1;
    } catch (const std::exception& e) {
        std::cerr << "Error occurred: " << e.what() << "\n";
        exit_code = 1;
    }

    curl_global_cleanup();
    return exit_code;
}
