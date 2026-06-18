#pragma once

#include <string>
#include <nlohmann/json.hpp>
#include "main.h"  // RpcConfig

// Thin JSON-RPC client for a Bitcoin Core node, backed by libcurl.
//
// Each instance owns a persistent CURL easy handle (stored as an opaque void*
// so that <curl/curl.h> does not leak into this header). Use call() for
// node-wide methods and wallet_call() for wallet-scoped methods (which Bitcoin
// Core exposes under the /wallet/<name> URL path).
class BitcoinRPC {
public:
    explicit BitcoinRPC(RpcConfig cfg);
    ~BitcoinRPC();

    // Non-copyable (owns a raw CURL handle), movable.
    BitcoinRPC(const BitcoinRPC&) = delete;
    BitcoinRPC& operator=(const BitcoinRPC&) = delete;
    BitcoinRPC(BitcoinRPC&& other) noexcept;
    BitcoinRPC& operator=(BitcoinRPC&& other) noexcept;

    // Call a node-wide RPC method; returns the "result" field on success.
    nlohmann::json call(const std::string& method,
                        const nlohmann::json& params = nlohmann::json::array());

    // Call a wallet-scoped RPC method against the given wallet.
    nlohmann::json wallet_call(const std::string& wallet,
                               const std::string& method,
                               const nlohmann::json& params = nlohmann::json::array());

private:
    // POST a JSON-RPC request body to the given URL and return the parsed
    // "result". Throws std::runtime_error on transport or JSON-RPC errors.
    nlohmann::json post(const std::string& url, const nlohmann::json& body);

    static size_t write_cb(char* ptr, size_t size, size_t nmemb, void* userdata);

    RpcConfig cfg_;
    void*     curl_ = nullptr;  // CURL* handle, kept opaque
};
