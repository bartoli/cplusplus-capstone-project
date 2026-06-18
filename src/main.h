#pragma once

#include <string>
#include <vector>
#include <nlohmann/json.hpp>

// ----------------------------------------------------------------------------
// Data structures
// ----------------------------------------------------------------------------

// Connection parameters for the Bitcoin Core JSON-RPC interface.
// Defaults match the regtest node defined in docker-compose.yaml / bitcoin.conf.
struct RpcConfig {
    std::string scheme = "http";
    std::string host   = "127.0.0.1";
    int         port   = 18443;
    std::string user   = "alice";
    std::string pass   = "password";
};

// One decoded transaction output (vout).
struct VoutEntry {
    std::string address;       // scriptPubKey.address (or addresses[0] on older nodes)
    double      value  = 0.0;  // amount in BTC
    long long   n      = 0;    // output index
};

// The 10 values written to out.txt, in the exact order required by the README.
struct TxReport {
    std::string txid;                  // 1. Transaction ID
    std::string miner_input_address;   // 2. Miner's input address
    double      miner_input_amount   = 0.0;  // 3. Miner's input amount (BTC)
    std::string trader_output_address;       // 4. Trader's output address
    double      trader_output_amount = 0.0;  // 5. Trader's output amount (BTC), expected 20
    std::string miner_change_address;        // 6. Miner's change address
    double      miner_change_amount  = 0.0;  // 7. Miner's change amount (BTC)
    double      fee                  = 0.0;  // 8. Transaction fee (BTC), negative per gettransaction
    long long   block_height         = 0;    // 9. Confirmation block height
    std::string block_hash;                  // 10. Confirmation block hash
};

// What to do with a wallet before using it (used by the idempotent setup logic).
enum class WalletAction { None, Create, Load };

// ----------------------------------------------------------------------------
// Pure helper functions (no network / no I/O — unit-testable in isolation)
// ----------------------------------------------------------------------------

// Build the base RPC URL, e.g. "http://127.0.0.1:18443".
std::string build_base_url(const RpcConfig& cfg);

// Build a wallet-scoped RPC URL, e.g. "http://127.0.0.1:18443/wallet/Miner".
std::string build_wallet_url(const RpcConfig& cfg, const std::string& wallet);

// Build a JSON-RPC 1.0 request body: {"jsonrpc":"1.0","id":..,"method":..,"params":..}.
nlohmann::json build_rpc_request(const std::string& method,
                                 const nlohmann::json& params,
                                 const std::string& id = "capstone");

// Decide whether a wallet must be created, loaded, or is already usable, given
// the lists returned by listwalletdir (on disk) and listwallets (loaded).
WalletAction decide_wallet_action(const std::vector<std::string>& on_disk,
                                  const std::vector<std::string>& loaded,
                                  const std::string& wallet);

// Parse the "vout" array of a decoderawtransaction result into VoutEntry values.
std::vector<VoutEntry> parse_vouts(const nlohmann::json& decoded_tx);

// Select the recipient output (the one paying the trader address). Throws
// std::runtime_error if no output matches.
VoutEntry select_recipient_vout(const std::vector<VoutEntry>& vouts,
                                const std::string& trader_address);

// Select the change output (the output that is NOT the trader's). Assumes
// exactly two outputs. Throws std::runtime_error if it cannot be identified.
VoutEntry select_change_vout(const std::vector<VoutEntry>& vouts,
                             const std::string& trader_address);

// Resolve a transaction input to its address/amount by indexing the decoded
// previous transaction's vout at input_vout. Throws if the index is invalid.
VoutEntry resolve_input_prevout(const nlohmann::json& prev_decoded,
                                long long input_vout);

// Format a BTC amount the same way Python's str(float) does (shortest round-trip,
// trailing ".0" stripped) so the output matches the sample, e.g. 20, 29.9999859,
// -1.41e-05.
std::string format_btc(double amount);

// Assemble the exact 10-line out.txt body (trailing newline after each line).
std::string format_report(const TxReport& r);
