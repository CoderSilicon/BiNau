#ifndef BINAUPARSER_HPP
#define BINAUPARSER_HPP

#include <vector>
#include <string>
#include <cstdint>

/**
 * @class BinauParser
 * @brief Handles high-performance static analysis of binary artifacts.
 */
class BinauParser {
public:
    BinauParser() = default;

    /**
     * Performs static deconstruction of the provided buffer.
     * @param data A managed vector containing the raw binary stream.
     * @return A serialized JSON string containing identified permissions and risks.
     */
    std::string perform_static_analysis(const std::vector<uint8_t>& data);

private:
    // Internal deconstruction methods
    bool is_pe_format(const std::vector<uint8_t>& data);
    bool is_elf_format(const std::vector<uint8_t>& data);
    
    // Header mapping logic
    void extract_iat(const std::vector<uint8_t>& data);

};

#endif // BINAUPARSER_HPP