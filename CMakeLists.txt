cmake_minimum_required(VERSION 3.14)
project(cplusplus_capstone CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

# --- GoogleTest (matches the sibling pathway exercise projects) ---
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.14.0
    GIT_SHALLOW    TRUE
)
# Keep CRT settings consistent on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# --- nlohmann/json: header-only JSON library used to build/parse JSON-RPC ---
# Use the packaged release tarball (small) rather than cloning the full git
# history, which keeps the first configure and CI fast.
FetchContent_Declare(
    nlohmann_json
    URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)

FetchContent_MakeAvailable(googletest nlohmann_json)

# --- libcurl: system HTTP client used to POST JSON-RPC to the Bitcoin node ---
find_package(CURL REQUIRED)

# --- Core library: pure helpers (main.cpp) + RPC client (rpc_client.cpp) ---
# Holds NO int main(); both the app and the unit tests link against it.
add_library(main src/main.cpp src/rpc_client.cpp)
target_include_directories(main PUBLIC src)
target_link_libraries(main PUBLIC CURL::libcurl nlohmann_json::nlohmann_json)

# --- Runnable program that talks to the node and produces out.txt ---
add_executable(capstone src/app.cpp)
target_link_libraries(capstone PRIVATE main)

# --- Unit tests for the pure helpers (no running node required) ---
enable_testing()
include(GoogleTest)

add_executable(test_main tests/main_test.cpp)
target_link_libraries(test_main PRIVATE main GTest::gtest_main)
gtest_discover_tests(test_main)
