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
    return cfg.scheme+"://"+cfg.host+":"+std::to_string(cfg.port);
}

std::string build_wallet_url(const RpcConfig& cfg, const std::string& wallet) {
    // TODO: return the base URL with "/wallet/<wallet>" appended.
    return build_base_url(cfg)+"/wallet/"+wallet;
}

nlohmann::json build_rpc_request(const std::string& method,
                                 const nlohmann::json& params,
                                 const std::string& id) {
    // TODO: build {"jsonrpc":"1.0","id":id,"method":method,"params":params}.
    return {{"jsonrpc","1.0"},{"id",id},{"method", method},{"params",params}};
}

WalletAction decide_wallet_action(const std::vector<std::string>& on_disk,
                                  const std::vector<std::string>& loaded,
                                  const std::string& wallet) {
    // TODO: Create if not on disk; Load if on disk but not loaded; else None.
    if(on_disk.empty())
      return WalletAction::Create;
    if(loaded.empty())
        return WalletAction::Load;
    (void) wallet;
    return WalletAction::None;
}

VoutEntry json_to_entry(const nlohmann::json& vout)
{
    auto spk = vout["scriptPubKey"];
    if(spk.contains("address"))
        return {spk["address"],vout["value"],vout["n"]};
    else
        return {spk["addresses"][0],vout["value"],vout["n"]};
}

std::vector<VoutEntry> parse_vouts(const nlohmann::json& decoded_tx) {
    // TODO: read each entry of decoded_tx["vout"] into a VoutEntry (value, n,
    //       and scriptPubKey.address with fallback to addresses[0]).
    std::vector<VoutEntry> out;
    for(const auto& vout : decoded_tx["vout"]){
        out.push_back(json_to_entry(vout));
    }
    return out;
}

VoutEntry select_recipient_vout(const std::vector<VoutEntry>& vouts,
                                const std::string& trader_address) {
    for(const auto& vout : vouts)
    {
        if(vout.address == trader_address)
            return VoutEntry(vout);
    }
    throw std::runtime_error("");
}

VoutEntry select_change_vout(const std::vector<VoutEntry>& vouts,
                             const std::string& trader_address) {
    for(const auto& vout : vouts)
    {
        if(vout.address != trader_address)
            return VoutEntry(vout);
    }
    throw std::runtime_error("");
}

VoutEntry resolve_input_prevout(const nlohmann::json& prev_decoded,
                                long long input_vout) {
    // TODO: index prev_decoded["vout"] at input_vout (throw if out of range).
    for (const auto vout : prev_decoded["vout"])
    {
        if(vout["n"] == input_vout)
          return json_to_entry(vout);
    }
    throw std::runtime_error("");
}

std::string format_btc(double amount) {
    // auto s = std::to_string(amount);
    // while (s.find('.', 1) != std::string::npos && s.back() == '0')
    //     s = s.substr(0, s.length()-1);
    // return s;

    bool is_neg = amount<0;
    if(is_neg)
      amount = -amount;

    //first round to nearest sat, double representation miuight not be exact same value
    int64_t sats = amount * 100000000;
    int64_t coins = sats / 100000000;
    int64_t cents = sats % 100000000;

    std::string out;
    if(is_neg)
        out +='-';
    out += std::to_string(coins);
    if(cents != 0)
    {
        out += '.';
        std::string sats_s = std::to_string(cents);
        while (sats_s.length()<8)
            sats_s = '0'+sats_s;
        while(!sats_s.empty() && sats_s.back()=='0')
          sats_s = sats_s.substr(0, sats_s.length()-1);
        out += sats_s;
    }
    return out;
}

std::string format_report(const TxReport& r) {
    std::string out;
    out += r.txid+'\n';
    out += r.miner_input_address+'\n';
    out += format_btc(r.miner_input_amount)+'\n';
    out += r.trader_output_address+'\n';
    out += format_btc(r.trader_output_amount)+'\n';
    out += r.miner_change_address+'\n';
    out += format_btc(r.miner_change_amount)+'\n';
    out += format_btc(r.fee)+'\n';
    out += std::to_string(r.block_height)+'\n';
    out += r.block_hash+'\n';
    
    return out;
}
