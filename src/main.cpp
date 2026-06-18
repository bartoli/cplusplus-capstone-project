// Pure helper functions for the capstone (no network / no I/O).
//
// These are unit-tested by tests/main_test.cpp without a running Bitcoin node.
// Implement each TODO so that the GoogleTest suite passes.

#include "main.h"

#include <algorithm>
#include <stdexcept>
#include <string>

std::string build_base_url(const RpcConfig& cfg) {
    // TODO: return "<scheme>://<host>:<port>", e.g. "http://127.0.0.1:18443".
    (void)cfg;
    return {};
}

std::string build_wallet_url(const RpcConfig& cfg, const std::string& wallet) {
    // TODO: return the base URL with "/wallet/<wallet>" appended.
    (void)cfg;
    (void)wallet;
    return {};
}

nlohmann::json build_rpc_request(const std::string& method,
                                 const nlohmann::json& params,
                                 const std::string& id) {
    // TODO: build {"jsonrpc":"1.0","id":id,"method":method,"params":params}.
    (void)method;
    (void)params;
    (void)id;
    return {};
}

WalletAction decide_wallet_action(const std::vector<std::string>& on_disk,
                                  const std::vector<std::string>& loaded,
                                  const std::string& wallet) {
    // TODO: Create if not on disk; Load if on disk but not loaded; else None.
    (void)on_disk;
    (void)loaded;
    (void)wallet;
    return WalletAction::None;
}

std::vector<VoutEntry> parse_vouts(const nlohmann::json& decoded_tx) {
    // TODO: read each entry of decoded_tx["vout"] into a VoutEntry (value, n,
    //       and scriptPubKey.address with fallback to addresses[0]).
    (void)decoded_tx;
    return {};
}

VoutEntry select_recipient_vout(const std::vector<VoutEntry>& vouts,
                                const std::string& trader_address) {
    // TODO: return the output whose address == trader_address (throw if none).
    (void)vouts;
    (void)trader_address;
    return {};
}

VoutEntry select_change_vout(const std::vector<VoutEntry>& vouts,
                             const std::string& trader_address) {
    // TODO: return the output whose address != trader_address (throw if none).
    (void)vouts;
    (void)trader_address;
    return {};
}

VoutEntry resolve_input_prevout(const nlohmann::json& prev_decoded,
                                long long input_vout) {
    // TODO: index prev_decoded["vout"] at input_vout (throw if out of range).
    (void)prev_decoded;
    (void)input_vout;
    return {};
}

std::string format_btc(double amount) {
    // TODO: shortest round-trip representation, trailing ".0" stripped.
    (void)amount;
    return {};
}

std::string format_report(const TxReport& r) {
    // TODO: produce the 10 out.txt lines in order, one per line.
    (void)r;
    return {};
}
