#!/bin/bash

mkdir -p build

set -e

echo "[+] Building stage 1 compiler"
gcc -o build/stage1 bootstrap/stage0.c
echo "[+] Building stage 2 compiler"
./build/stage1 compiler/main.ae -o build/stage2
echo "[+] Building stage 3 compiler"
./build/stage2 compiler/main.ae -o build/stage3
echo "[+] Comparing stage 2 and 3"
# diff build/stage2.c build/stage3.c
if diff build/stage2.c build/stage3.c; then
    echo "[+] Bootstrap successful: Use ./bootstrap/aecor"
else
    echo "[-] Error: Stage 2 and 3 are different"
    exit 1
fi
mv build/stage3 bootstrap/aecor