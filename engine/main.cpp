#include <emscripten.h>
#include <stdint.h>
#include "include/parser.hpp"

extern "C"
{

    EMSCRIPTEN_KEEPALIVE
    void analyze_binary(const uint8_t *buffer, size_t size)
    {
        std::cout << "\n--- Wasm Engine Booting ---\n";

        Engine analyzer;

        if (analyzer.loadBuffer(buffer, size))
        {
            if (!analyzer.analyzeOS())
            {
                std::cout << "[HALT] Unsupported OS.\n";
                return;
            }

            analyzer.getCompileTime();
            analyzer.checkRelocations();
            analyzer.scanSections();
            analyzer.checkOverlay();
            analyzer.scanImports();
            analyzer.scanExports();
            analyzer.checkDigitalSignature();
            analyzer.scanAntiAnalysis();
        }

        std::cout << "--- Analysis Complete ---\n";
    }
}