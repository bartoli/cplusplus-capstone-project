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
#include <fstream>
int main() {
    curl_global_init(CURL_GLOBAL_DEFAULT);

    int exit_code = 0;
    try {
        RpcConfig cfg;  // defaults: alice:password @ 127.0.0.1:18443
        BitcoinRPC client(cfg);

        //   1. Create/load the "Miner" and "Trader" wallets (idempotent).
        {
            nlohmann::json params{"Miner"};
            auto rep = client.call("createwallet", params);
            std::cout<<rep.dump()<<std::endl;    
            params = {"Trader"};
            rep = client.call("createwallet", params);
            std::cout<<rep.dump()<<std::endl;
        }

        //   2. Generate a Miner address (label "Mining Reward") and mine 101 blocks to
        //      it so the first coinbase reward matures (100-confirmation rule).
        std::string mine_addr;
        {
            nlohmann::json params;
            params["label"] = "Mining reward";
            auto rep = client.wallet_call("Miner", "getnewaddress", params);
            mine_addr = rep["result"];
            std::cout<<rep.dump()<<std::endl;
            params = nlohmann::json();
            params["nblocks"] = 101;
            params["address"] = mine_addr;
            
            /* rep = */client.call("generatetoaddress", params);
            //std::cout<<rep.dump()<<std::endl;
        }

        //   3. Print the Miner balance.
        auto rep = client.wallet_call("Miner", "getbalance");
        std::cout<<rep.dump()<<std::endl;
        auto miner_balance = rep["result"];

        //   4. Generate a Trader receiving address (label "Received").
        std::string trader_addr;
        {
            nlohmann::json params;
            params["label"] = "Received";
            auto rep = client.wallet_call("Trader", "getnewaddress", params);
            trader_addr = rep["result"];
            std::cout<<rep.dump()<<std::endl;
        }

        //   5. Send 20 BTC from Miner to Trader.
        std::string txid;
        {
            nlohmann::json params;
            params["address"] = trader_addr;
            params["amount"] = 20;
            auto rep = client.wallet_call("Miner","sendtoaddress",params);
            std::cout<<rep.dump()<<std::endl;
            txid = rep["result"];
        }

        //   6. Confirm the transaction is in the mempool.
        double fees;
        {
            nlohmann::json params;
            params["txid"] = txid;
            auto rep = client.call("getmempoolentry",params);
            std::cout<<rep.dump()<<std::endl;
            fees = rep["result"]["fees"]["base"];
            //txid = rep["result"]["wtxid"];
        }

        //   7. Mine 1 block to confirm it.
        double trader_balance;
        {
            nlohmann::json params;
            params = nlohmann::json();
            params["nblocks"] = 1;
            params["address"] = mine_addr;            
            /* rep = */client.call("generatetoaddress", params);
            auto rep = client.wallet_call("Trader", "getbalance");
            std::cout<<rep.dump()<<std::endl;
            trader_balance = rep["result"];
        }

        std::string change_addr;
        double change_amt;
        {
            //get the confirmed transaction
            nlohmann::json params;  
            params["txid"] = txid;
            auto rep = client.wallet_call("Miner", "gettransaction", params);
            auto tx_hex = rep["result"]["hex"];
            std::cout<<rep.dump()<<std::endl;

            params={};
            params["hexstring"] = tx_hex;
            rep = client.call("decoderawtransaction", params);
            std::cout<<rep.dump()<<std::endl;
            auto change_vout = rep["result"]["vout"][0];
            change_addr = change_vout["scriptPubKey"]["address"];
            change_amt = change_vout["value"];            
        }
        
        int height;
        std::string block_hash;
        {
            nlohmann::json params;
            auto rep = client.call("getblockchaininfo",params);
            std::cout<<rep.dump()<<std::endl;
            height = rep["result"]["blocks"];
            block_hash = rep["result"]["bestblockhash"];        
        }

        //   8. Fetch transaction details (gettransaction / getrawtransaction /
        //      decoderawtransaction) and write the report to out.txt.
        std::ofstream of("out.txt", std::ios::out);
        of << txid <<std::endl;
        of << mine_addr << std::endl;
        of << miner_balance<<std::endl;
        of << trader_addr<<std::endl;
        of << trader_balance<<std::endl;
        of << change_addr<<std::endl;
        of << change_amt<<std::endl;
        of << fees <<std::endl;
        of << height <<std::endl;
        of << block_hash << std::endl;
        of.close();
    } catch (const std::exception& e) {
        std::cerr << "Error occurred: " << e.what() << "\n";
        exit_code = 1;
    }

    curl_global_cleanup();
    return exit_code;
}
