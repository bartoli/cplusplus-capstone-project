#!/bin/bash
# Build the capstone program and run it. Produces out.txt at the repo root.
set -e

# Configure and build (downloads GoogleTest + nlohmann/json on first run).
cmake -S . -B build
cmake --build build --target capstone

# Run the program. It connects to the regtest node on 127.0.0.1:18443.
./build/capstone
