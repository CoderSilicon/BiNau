# scripts/build_wasm.ps1
Write-Host "Compiling wasm (windows)..." -ForegroundColor Cyan

# 1. Ensure output directory exists
if (!(Test-Path "../static")) { New-Item -ItemType Directory -Path "../static" }

# 2. Run Emscripten Compiler
emcc ../engine/src/main.cpp ../engine/src/parser.cpp `
    -I../engine/include `
    -o ../static/binau_core.js `
    -s WASM=1 `
    -s MODULARIZE=1 `
    -s EXPORT_ES6=1 `
    -s ALLOW_MEMORY_GROWTH=1 `
    -O3 # Maximum optimization for your Ryzen 9

Write-Host "Compilation Complete. Artifacts moved to /static." -ForegroundColor Green