#!/bin/bash

CYAN='\033[0;36m'
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' 

echo -e "${CYAN}Compiling wasm (linux)...${NC}"

# 2. Verify Emscripten (emcc) is in the PATH
if ! command -v emcc &> /dev/null
then
    echo -e "${RED}Error: emcc not found. Please source your emsdk environment.${NC}"
    exit 1
fi

# 3. Path Configuration
SOURCE_DIR="../engine/src"
INCLUDE_DIR="../engine/include"
OUTPUT_DIR="../static"

# 4. Compilation Execution
# -O3: Maximum optimization for the Ryzen 9 9950X
# ALLOW_MEMORY_GROWTH: Ensures the auditor can handle larger binaries
emcc "$SOURCE_DIR/main.cpp" "$SOURCE_DIR/parser.cpp" \
    -I"$INCLUDE_DIR" \
    -o "$OUTPUT_DIR/binau_core.js" \
    -s WASM=1 \
    -s MODULARIZE=1 \
    -s EXPORT_ES6=1 \
    -s ALLOW_MEMORY_GROWTH=1 \
    -O3

# 5. Result Verification
if [ $? -eq 0 ]; then
    echo -e "${GREEN}Build Successful.${NC} Artifacts generated in $OUTPUT_DIR"
else
    echo -e "${RED}Build Failed.${NC} Check C++ compiler errors above."
    exit 1
fi