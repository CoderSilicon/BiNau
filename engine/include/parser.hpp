#ifndef PARSER_HPP
#define PARSER_HPP

#include <iostream>
#include <string>
#include <vector>
#include <cstdint>

// 1. Manually define the PE structures to replace <windows.h>
#pragma pack(push, 1)
struct IMAGE_DOS_HEADER
{
    uint16_t e_magic;
    uint16_t e_cblp, e_cp, e_crlc, e_cparhdr, e_minalloc, e_maxalloc, e_ss, e_sp, e_csum, e_ip, e_cs, e_lfarlc, e_ovno;
    uint16_t e_res[4];
    uint16_t e_oemid, e_oeminfo;
    uint16_t e_res2[10];
    uint32_t e_lfanew;
};

struct IMAGE_SECTION_HEADER
{
    uint8_t Name[8];
    union
    {
        uint32_t PhysicalAddress;
        uint32_t VirtualSize;
    } Misc;
    uint32_t VirtualAddress;
    uint32_t SizeOfRawData;
    uint32_t PointerToRawData;
    uint32_t PointerToRelocations;
    uint32_t PointerToLinenumbers;
    uint16_t NumberOfRelocations;
    uint16_t NumberOfLinenumbers;
    uint32_t Characteristics;
};

struct IMAGE_IMPORT_DESCRIPTOR
{
    union
    {
        uint32_t Characteristics;
        uint32_t OriginalFirstThunk;
    };
    uint32_t TimeDateStamp;
    uint32_t ForwarderChain;
    uint32_t Name;
    uint32_t FirstThunk;
};
#pragma pack(pop)

class Engine
{
private:
    std::string filePath;
    std::vector<char> headerBuffer;
    std::streamsize fileSize;
    std::string detectedOS;

    void printHex(size_t count);
    double calculateEntropy(size_t offset, size_t size);

public:
    Engine();
    Engine(const std::string &path);

    bool loadFile();
    bool loadBuffer(const uint8_t *buffer, size_t size);

    bool analyzeOS();
    void setHeaderBuffer(const char* data, size_t size);
    void scanSections();
    uint32_t rvaToOffset(uint32_t rva);
    void scanImports();
    void checkDigitalSignature();
    void checkOverlay();
    void scanExports();
    void dumpEntryPoint();
    void scanAntiAnalysis();
    void checkRelocations();
    void getCompileTime();
};

#endif