// JSON-RPC client for Bitcoin Core, backed by libcurl.
//
// Implement each TODO so the program can talk to the node.

#include "rpc_client.h"

#include <curl/curl.h>

#include <stdexcept>
#include <string>
#include <utility>
#include <iostream>

BitcoinRPC::BitcoinRPC(RpcConfig cfg) : cfg_(std::move(cfg)) {
    curl_ =  curl_easy_init();
}

BitcoinRPC::~BitcoinRPC() {
    if(curl_)
        curl_easy_cleanup(curl_);
}

BitcoinRPC::BitcoinRPC(BitcoinRPC&& other) noexcept
    : cfg_(std::move(other.cfg_)), curl_(other.curl_) {

    other.curl_ = nullptr;
}

BitcoinRPC& BitcoinRPC::operator=(BitcoinRPC&& other) noexcept {
    if(curl_)
        curl_easy_cleanup(curl_);
    curl_ = other.curl_;
    return *this;
}

size_t BitcoinRPC::write_cb(char* ptr, size_t size, size_t nmemb, void* userdata) {
    // TODO: append (size * nmemb) bytes from ptr to the std::string in userdata
    //       and return the number of bytes handled.
    memcpy(userdata, ptr, size*nmemb);
    return size * nmemb;
}

nlohmann::json BitcoinRPC::post(const std::string& url, const nlohmann::json& body) {
    // TODO: POST body.dump() to url with a JSON content-type header and HTTP
    //       basic auth; capture the response via write_cb; parse it; raise on a
    //       non-null JSON-RPC "error"; return the "result" field.

    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    curl_easy_setopt(curl_, CURLOPT_HTTPHEADER, headers);

    curl_easy_setopt(curl_, CURLOPT_USERNAME, cfg_.user.c_str());
    curl_easy_setopt(curl_, CURLOPT_PASSWORD, cfg_.pass.c_str());
    curl_easy_setopt(curl_, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);

    //std::cout << url << '\n';
    curl_easy_setopt(curl_, CURLOPT_URL, url.c_str());
    auto json = body.dump();
    curl_easy_setopt(curl_, CURLOPT_POSTFIELDS, json.c_str());
     curl_easy_setopt(curl_, CURLOPT_POSTFIELDSIZE, static_cast<long>(json.size()));

    char response[20480]={0};
    curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, write_cb);
    curl_easy_setopt(curl_, CURLOPT_WRITEDATA, response);
    std::cout<<response<<std::endl;

    CURLcode res = curl_easy_perform(curl_);
    if (res != CURLE_OK)
    {
        std::cerr << curl_easy_strerror(res) << '\n';
    }

    return nlohmann::json::parse(response);
}

nlohmann::json BitcoinRPC::call(const std::string& method, const nlohmann::json& params) {
    // TODO: post(build_base_url(cfg_), build_rpc_request(method, params)).
    return post(build_base_url(cfg_),build_rpc_request(method, params));
}

nlohmann::json BitcoinRPC::wallet_call(const std::string& wallet,
                                       const std::string& method,
                                       const nlohmann::json& params) {
    return post(build_wallet_url(cfg_, wallet), build_rpc_request(method, params));
}
