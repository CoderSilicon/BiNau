#include <emscripten.h>
#include <iostream>
#include <string>
#include <vector>
#include "../include/parser.hpp" // Assumed header for your logic in parser.cpp

extern "C" {

/**
 * Primary entry point for binary deconstruction.
 * @param buffer Pointer to the raw byte array passed from Svelte.
 * @param size The total size of the binary artifact.
 * @return A serialized JSON string containing the Permission Matrix.
 */
EMSCRIPTEN_KEEPALIVE
const char* analyze_artifact(uint8_t* buffer, int size) {

    std::vector<uint8_t> binary_data(buffer, buffer + size);


    BinauParser parser;
    std::string analysis_report = parser.perform_static_analysis(binary_data);

    static std::string report_storage;
    report_storage = analysis_report;

    return report_storage.c_str();
}

}