#include <emscripten.h>
#include <stdint.h>
#include <iostream>
#include "./include/parser.hpp"

extern "C" {

    EMSCRIPTEN_KEEPALIVE
    void analyze_binary(const uint8_t* buffer, size_t size) 
    {
        std::cout << "\n--- Wasm Analysis Engine Initiated ---\n";
        
        Engine analyzer;

        if (analyzer.loadBuffer(buffer, size)) 
        {
            analyzer.analyzeOS();
            analyzer.getCompileTime();
            analyzer.checkRelocations();
            analyzer.scanSections();
            analyzer.checkOverlay();
            analyzer.scanImports();
            analyzer.scanExports();
            analyzer.checkDigitalSignature();
            analyzer.scanAntiAnalysis();
        }

        std::cout << "--- Analysis Routine Complete ---\n";
    }
}