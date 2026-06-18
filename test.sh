#!/bin/bash
# Full local integration test: spins up a regtest bitcoind, builds and runs the
# capstone program to produce out.txt, then runs the language-agnostic Jest
# autograder against the node. Mirrors the Python capstone's test.sh.

# Install the Node dependencies for the Jest autograder.
npm install

set -e  # Exit immediately if any command fails

# Spawn bitcoind in regtest mode.
docker compose up -d
sleep 10

echo "Waiting for bitcoind to be fully initialized..."

while true; do
  result=$(curl --silent --user alice:password --data-binary \
    '{"jsonrpc":"1.0","id":"ping","method":"getblockchaininfo","params":[]}' \
    -H 'content-type: text/plain;' http://127.0.0.1:18443)

  if echo "$result" | grep -q '"chain"'; then
    echo "bitcoind is ready."
    break
  else
    echo "bitcoind not ready yet, retrying in 3s..."
    sleep 3
  fi
done

chmod +x ./run.sh

# Build + run the C++ program (produces out.txt), then run the autograder.
/bin/bash run.sh
npm run test

# Stop the docker container.
docker compose down -v
