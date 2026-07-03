#!/bin/bash
set -e

echo "Checking for 32-bit compilation support..."
if ! gcc -m32 -E - </dev/null >/dev/null 2>&1; then
    echo "ERROR: 32-bit compilation not available."
    echo "Install gcc-multilib: sudo apt install gcc-multilib"
    exit 1
fi

gcc -o server server.c secrets.c \
    -Wall -O0 -g \
    -std=gnu99 \
    -fno-stack-protector \
    -m32 \
    -no-pie

echo "Build successful: ./server"
