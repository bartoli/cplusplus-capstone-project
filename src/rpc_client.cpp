// JSON-RPC client for Bitcoin Core, backed by libcurl.
//
// Implement each TODO so the program can talk to the node.

#include "rpc_client.h"

#include <curl/curl.h>

#include <stdexcept>
#include <string>
#include <utility>

BitcoinRPC::BitcoinRPC(RpcConfig cfg) : cfg_(std::move(cfg)) {
    // TODO: initialise a libcurl easy handle into curl_ (throw on failure).
}

BitcoinRPC::~BitcoinRPC() {
    // TODO: clean up the libcurl easy handle if non-null.
}

BitcoinRPC::BitcoinRPC(BitcoinRPC&& other) noexcept
    : cfg_(std::move(other.cfg_)), curl_(other.curl_) {
    other.curl_ = nullptr;
}

BitcoinRPC& BitcoinRPC::operator=(BitcoinRPC&& other) noexcept {
    // TODO: free any existing handle, then steal other's handle/config.
    (void)other;
    return *this;
}

size_t BitcoinRPC::write_cb(char* ptr, size_t size, size_t nmemb, void* userdata) {
    // TODO: append (size * nmemb) bytes from ptr to the std::string in userdata
    //       and return the number of bytes handled.
    (void)ptr;
    (void)userdata;
    return size * nmemb;
}

nlohmann::json BitcoinRPC::post(const std::string& url, const nlohmann::json& body) {
    // TODO: POST body.dump() to url with a JSON content-type header and HTTP
    //       basic auth; capture the response via write_cb; parse it; raise on a
    //       non-null JSON-RPC "error"; return the "result" field.
    (void)url;
    (void)body;
    return {};
}

nlohmann::json BitcoinRPC::call(const std::string& method, const nlohmann::json& params) {
    // TODO: post(build_base_url(cfg_), build_rpc_request(method, params)).
    (void)method;
    (void)params;
    return {};
}

nlohmann::json BitcoinRPC::wallet_call(const std::string& wallet,
                                       const std::string& method,
                                       const nlohmann::json& params) {
    // TODO: post(build_wallet_url(cfg_, wallet), build_rpc_request(method, params)).
    (void)wallet;
    (void)method;
    (void)params;
    return {};
}
