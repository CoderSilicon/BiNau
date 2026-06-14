#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <cstring>
#include <iomanip>
#include <cstdint>
#include "include/parser.hpp"

Engine::Engine() : filePath(""), fileSize(0), detectedOS("Unknown") {}

void Engine::printHex(size_t count)
{
    for (size_t i = 0; i < count && i < headerBuffer.size(); ++i)
    {
        std::cout << std::hex << (static_cast<int>(headerBuffer[i]) & 0xFF) << " ";
    }
    std::cout << std::dec << "\n";
}

bool Engine::loadBuffer(const uint8_t *buffer, size_t size)
{
    if (size < 64)
    {
        std::cerr << "[-] Error: Buffer data size is too small.\n";
        return false;
    }
    fileSize = size;
    headerBuffer.assign(buffer, buffer + size);
    return true;
}

double Engine::calculateEntropy(size_t offset, size_t size)
{
    if (size == 0 || offset + size > headerBuffer.size())
        return 0.0;

    size_t counts[256] = {0};
    for (size_t i = offset; i < offset + size; ++i)
    {
        counts[static_cast<unsigned char>(headerBuffer[i])]++;
    }

    double entropy = 0.0;
    for (int i = 0; i < 256; ++i)
    {
        if (counts[i] > 0)
        {
            double probability = static_cast<double>(counts[i]) / size;
            entropy -= probability * std::log2(probability);
        }
    }
    return entropy;
}

bool Engine::loadFile()
{
    std::ifstream file(filePath, std::ios::binary | std::ios::ate);
    if (!file.is_open())
    {
        std::cerr << "[-] Error: Could not open file " << filePath << "\n";
        return false;
    }

    fileSize = file.tellg();
    if (fileSize < 64)
    {
        std::cerr << "[-] Error: File is too small to be a valid executable.\n";
        return false;
    }

    // Allocate buffer for the ENTIRE file
    headerBuffer.resize(fileSize);

    file.seekg(0, std::ios::beg);
    file.read(headerBuffer.data(), fileSize);

    return true;
}

bool Engine::analyzeOS()
{
    std::cout << "Analyzing...\n";
    if (headerBuffer.size() < 4)
    {
        std::cout << "Format: Unknown (File is too small or empty).\n";
        return false;
    }

    if (headerBuffer[0] == 0x4D && headerBuffer[1] == 0x5A)
    {
        detectedOS = "Windows";
        std::cout << "Format: Windows Portable Executable (.EXE)\n";
        return true;
    }
    else if (headerBuffer[0] == 0x7F && headerBuffer[1] == 0x45 && headerBuffer[2] == 0x4C && headerBuffer[3] == 0x46) // ELF
    {
        detectedOS = "Linux";
        std::cout << "Format: Linux ELF Binary\n";
        return false;
    }
    else if ((headerBuffer[0] == '\xCF' && headerBuffer[1] == '\xFA' && headerBuffer[2] == '\xED' && headerBuffer[3] == '\xFE') ||
             (headerBuffer[0] == '\xFE' && headerBuffer[1] == '\xED' && headerBuffer[2] == '\xFA' && headerBuffer[3] == '\xCF')) // Mach-O
    {
        detectedOS = "macOS";
        std::cout << "Format: macOS Mach-O Binary\n";
        return false;
    }
    else
    {
        std::cout << "Format: Unknown. ";
        std::cerr << "\nUnknown format found at input, terminating process\n";
        printHex(4);
        return false;
    }
}

void Engine::setHeaderBuffer(const char *data, size_t size)
{
    headerBuffer.assign(data, data + size);
    fileSize = size;
}
void Engine::scanSections()
{
    std::cout << "Windows\n";

    if (detectedOS != "Windows")
    {
        std::cout << "   Currently supports only PE format\n";
        return;
    }

    if (headerBuffer.size() < sizeof(IMAGE_DOS_HEADER))
        return;
    auto *dosHeader = reinterpret_cast<IMAGE_DOS_HEADER *>(headerBuffer.data());

    uint32_t peOffset = dosHeader->e_lfanew;

    if (peOffset + 24 > headerBuffer.size() ||
        headerBuffer[peOffset] != 'P' ||
        headerBuffer[peOffset + 1] != 'E')
    {
        std::cout << "    Error: Invalid signature\n";
        return;
    }

    uint16_t sectionCount = *reinterpret_cast<uint16_t *>(&headerBuffer[peOffset + 6]);
    uint16_t sizeOfOptionalHeader = *reinterpret_cast<uint16_t *>(&headerBuffer[peOffset + 20]);
    uint32_t sectionTableOffset = peOffset + 24 + sizeOfOptionalHeader;

    std::cout << "    [+] Found " << sectionCount << " sections. Analyzing...\n\n";

    for (int i = 0; i < sectionCount; ++i)
    {
        uint32_t currentSectionOffset = sectionTableOffset + (i * sizeof(IMAGE_SECTION_HEADER));

        if (currentSectionOffset + sizeof(IMAGE_SECTION_HEADER) > headerBuffer.size())
            break;

        auto *section = reinterpret_cast<IMAGE_SECTION_HEADER *>(&headerBuffer[currentSectionOffset]);

        char name[9] = {0};
        std::memcpy(name, section->Name, 8);

        std::cout << "    -> Section: " << name << "\n";

        double entropy = calculateEntropy(section->PointerToRawData, section->SizeOfRawData);
        std::cout << "       Entropy: " << std::fixed << std::setprecision(2) << entropy;

        if (entropy > 7.2)
        {
            std::cout << " High:\n";
        }
        else
        {
            std::cout << " Normal:\n";
        }

        bool isWritable = (section->Characteristics & 0x80000000) != 0;
        bool isExecutable = (section->Characteristics & 0x20000000) != 0;

        if (isWritable && isExecutable)
        {
            std::cout << "       [CRITICAL: W^X VIOLATION - MEMORY INJECTION VECTOR]\n";
        }

        // Note: Using section->Misc.VirtualSize here for win5.h compatibility
        if (section->SizeOfRawData == 0 && section->Misc.VirtualSize > 0)
        {
            std::cout << "       [WARNING: EMPTY ON DISK BUT ALLOCATES MEMORY - POSSIBLE UNPACKER STUB]\n";
        }
    }
}

void Engine::checkDigitalSignature()
{
    std::cout << "Checking Digital Signature...\n";

    if (detectedOS != "Windows")
        return;

    auto *dosHeader = reinterpret_cast<IMAGE_DOS_HEADER *>(headerBuffer.data());
    uint32_t peOffset = dosHeader->e_lfanew;

    // Read the Magic Number to determine if it's 32-bit or 64-bit
    uint16_t magic = *reinterpret_cast<uint16_t *>(&headerBuffer[peOffset + 24]);

    uint32_t securityDirOffset = 0;
    uint32_t securityDirSize = 0;

    // The Security Directory is always Index 4 in the Data Directories array.
    if (magic == 0x10B) // 32-bit (PE32)
    {
        securityDirOffset = *reinterpret_cast<uint32_t *>(&headerBuffer[peOffset + 152]);
        securityDirSize = *reinterpret_cast<uint32_t *>(&headerBuffer[peOffset + 156]);
    }
    else if (magic == 0x20B) // 64-bit (PE32+)
    {
        securityDirOffset = *reinterpret_cast<uint32_t *>(&headerBuffer[peOffset + 168]);
        securityDirSize = *reinterpret_cast<uint32_t *>(&headerBuffer[peOffset + 172]);
    }

    // PE Format Quirk: The Security Directory "VirtualAddress" is actually a direct file offset, not an RVA.
    if (securityDirOffset != 0 && securityDirSize != 0 && securityDirOffset < headerBuffer.size())
    {
        std::cout << "    -> Signature block found (Offset: " << securityDirOffset << ", Size: " << securityDirSize << " bytes).\n";
        std::cout << "    -> Note: Cryptographic verification of the certificate requires the WinTrust API.\n";
    }
    else
    {
        std::cout << "    -> No digital signature found. File is unsigned.\n";
    }
}

void Engine::scanAntiAnalysis()
{
    std::cout << "Scanning for Anti-Analysis Indicators...\n";

    std::vector<std::string> suspiciousStrings = {
        "VBoxGuest",
        "vboxmrxnp",
        "VBoxGuestAdditions",
        "VBoxMouse",
        "vmtoolsd",
        "vmware",
        "qemu-ga",
        "XenGuest",
        "kvm",
        "VMBus",
        "wine_get_version",
        "cuckoo",
        "joebox",
        "SbieSvc",
        "sbiedll.dll",

        "ollydbg",
        "x64dbg",
        "windbg",
        "immunity",
        "procmon",
        "wireshark",
        "dumpcap",
        "fiddler",

        "IsDebuggerPresent",
        "CheckRemoteDebuggerPresent",
        "OutputDebugStringA",
        "FindWindowA",
        "EnumProcesses",
        "NtQueryInformationProcess",

        "SOFTWARE\\VMware, Inc.\\VMware Tools",
        "HARDWARE\\Description\\System\\SystemBiosVersion",
        "HARDWARE\\DEVICEMAP\\Scsi\\Scsi Port 0\\Scsi Bus 0\\Target Id 0\\Logical Unit Id 0"};

    std::string fileContent(headerBuffer.data(), headerBuffer.size());
    int hits = 0;

    for (const auto &indicator : suspiciousStrings)
    {
        if (fileContent.find(indicator) != std::string::npos)
        {
            std::cout << "    -> Indicator found: " << indicator << "\n";
            hits++;
        }
    }

    if (hits == 0)
    {
        std::cout << "    -> No known anti-analysis strings detected.\n";
    }
}
void Engine::checkOverlay()
{
    std::cout << "Checking file boundaries...\n";

    if (detectedOS != "Windows" || headerBuffer.size() < sizeof(IMAGE_DOS_HEADER))
        return;

    auto *dosHeader = reinterpret_cast<IMAGE_DOS_HEADER *>(headerBuffer.data());
    uint32_t peOffset = dosHeader->e_lfanew;
    uint16_t sectionCount = *reinterpret_cast<uint16_t *>(&headerBuffer[peOffset + 6]);
    uint16_t sizeOfOptionalHeader = *reinterpret_cast<uint16_t *>(&headerBuffer[peOffset + 20]);
    uint32_t sectionTableOffset = peOffset + 24 + sizeOfOptionalHeader;

    uint32_t expectedFileSize = 0;

    for (int i = 0; i < sectionCount; ++i)
    {
        auto *section = reinterpret_cast<IMAGE_SECTION_HEADER *>(&headerBuffer[sectionTableOffset + (i * sizeof(IMAGE_SECTION_HEADER))]);
        uint32_t sectionEndOnDisk = section->PointerToRawData + section->SizeOfRawData;

        if (sectionEndOnDisk > expectedFileSize)
        {
            expectedFileSize = sectionEndOnDisk;
        }
    }

    std::cout << "    -> Declared size: " << expectedFileSize << " bytes\n";
    std::cout << "    -> Actual size: " << fileSize << " bytes\n";

    if (fileSize > expectedFileSize)
    {
        std::streamsize overlaySize = fileSize - expectedFileSize;
        std::cout << "    -> Overlay detected: " << overlaySize << " bytes of unmapped data appended to the file.\n";
    }
    else
    {
        std::cout << "    -> No overlay detected.\n";
    }
}

uint32_t Engine::rvaToOffset(uint32_t rva)
{
    if (rva == 0)
        return 0;

    auto *dosHeader = reinterpret_cast<IMAGE_DOS_HEADER *>(headerBuffer.data());
    uint32_t peOffset = dosHeader->e_lfanew;
    uint16_t sectionCount = *reinterpret_cast<uint16_t *>(&headerBuffer[peOffset + 6]);
    uint16_t sizeOfOptionalHeader = *reinterpret_cast<uint16_t *>(&headerBuffer[peOffset + 20]);
    uint32_t sectionTableOffset = peOffset + 24 + sizeOfOptionalHeader;

    for (int i = 0; i < sectionCount; ++i)
    {
        auto *section = reinterpret_cast<IMAGE_SECTION_HEADER *>(&headerBuffer[sectionTableOffset + (i * sizeof(IMAGE_SECTION_HEADER))]);

        uint32_t sectionStart = section->VirtualAddress;
        uint32_t sectionSize = section->Misc.VirtualSize > 0 ? section->Misc.VirtualSize : section->SizeOfRawData;
        uint32_t sectionEnd = sectionStart + sectionSize;

        if (rva >= sectionStart && rva < sectionEnd)
        {
            return (rva - sectionStart) + section->PointerToRawData;
        }
    }
    return 0;
}

void Engine::scanImports()
{
    std::cout << "Parsing Import Address Table...\n";
    if (detectedOS != "Windows")
        return;

    auto *dosHeader = reinterpret_cast<IMAGE_DOS_HEADER *>(headerBuffer.data());
    uint32_t peOffset = dosHeader->e_lfanew;

    uint16_t magic = *reinterpret_cast<uint16_t *>(&headerBuffer[peOffset + 24]);
    uint32_t importDirectoryRVA = 0;

    if (magic == 0x10B) // 32-bit
    {
        importDirectoryRVA = *reinterpret_cast<uint32_t *>(&headerBuffer[peOffset + 128]);
    }
    else if (magic == 0x20B) // 64-bit
    {
        importDirectoryRVA = *reinterpret_cast<uint32_t *>(&headerBuffer[peOffset + 144]);
    }

    if (importDirectoryRVA == 0)
    {
        std::cout << "    -> No imports found.\n";
        return;
    }

    uint32_t importOffset = rvaToOffset(importDirectoryRVA);
    if (importOffset == 0 || importOffset >= headerBuffer.size())
        return;

    auto *importDescriptor = reinterpret_cast<IMAGE_IMPORT_DESCRIPTOR *>(&headerBuffer[importOffset]);

    int dllCount = 0;
    // Safety constraint: Protect against malformed headers or WASM memory drift
    while (importDescriptor->Name != 0 && dllCount < 1000)
    {
        uint32_t nameOffset = rvaToOffset(importDescriptor->Name);

        // Ensure the offset points inside our valid memory buffer bounds
        if (nameOffset > 0 && nameOffset < headerBuffer.size())
        {
            std::string dllName(&headerBuffer[nameOffset]);
            std::cout << "    -> Dependency: " << dllName << "\n";
            dllCount++;
        }
        else
        {
            break; // Break instantly if pointers jump outside file scope
        }
        importDescriptor++;
    }

    std::cout << "    -> Total loaded DLLs: " << dllCount << "\n";
}

void Engine::dumpEntryPoint()
{
    std::cout << "Analyzing Address of Entry Point (AoEP)...\n";
    if (detectedOS != "Windows")
        return;

    auto *dosHeader = reinterpret_cast<IMAGE_DOS_HEADER *>(headerBuffer.data());
    uint32_t peOffset = dosHeader->e_lfanew;

    uint32_t entryPointRVA = *reinterpret_cast<uint32_t *>(&headerBuffer[peOffset + 40]);
    std::cout << "    -> Entry Point RVA: 0x" << std::hex << entryPointRVA << std::dec << "\n";

    uint32_t entryOffset = rvaToOffset(entryPointRVA);
    if (entryOffset == 0 || entryOffset >= headerBuffer.size())
    {
        std::cout << "    -> Warning: Entry Point maps to an invalid file offset.\n";
        return;
    }

    std::cout << "    -> Entry Point File Offset: 0x" << std::hex << entryOffset << std::dec << "\n";
    std::cout << "    -> Execution Code Prologue (First 16 Bytes): ";

    std::ios_base::fmtflags f(std::cout.flags());

    for (size_t i = 0; i < 16 && (entryOffset + i) < headerBuffer.size(); ++i)
    {
        std::cout << std::setw(2) << std::setfill('0') << std::hex
                  << (static_cast<int>(headerBuffer[entryOffset + i]) & 0xFF) << " ";
    }

    std::cout.flags(f);
    std::cout << "\n";
}

void Engine::getCompileTime()
{
    std::cout << "Extracting Compilation Metadata...\n";
    if (detectedOS != "Windows")
        return;

    auto *dosHeader = reinterpret_cast<IMAGE_DOS_HEADER *>(headerBuffer.data());
    uint32_t peOffset = dosHeader->e_lfanew;

    uint32_t timeDateStamp = *reinterpret_cast<uint32_t *>(&headerBuffer[peOffset + 8]);

    time_t compileTime = static_cast<time_t>(timeDateStamp);
    struct tm *timeinfo = std::gmtime(&compileTime);

    if (timeinfo)
    {
        char buffer[80];
        std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S UTC", timeinfo);
        std::cout << "    -> Compiler Timestamp: " << buffer << "\n";
    }
    else
    {
        std::cout << "    -> Compiler Timestamp: Invalid or Obfuscated\n";
    }
}

void Engine::checkRelocations()
{
    std::cout << "Checking Base Relocations...\n";
    if (detectedOS != "Windows")
        return;

    auto *dosHeader = reinterpret_cast<IMAGE_DOS_HEADER *>(headerBuffer.data());
    uint32_t peOffset = dosHeader->e_lfanew;

    uint16_t characteristics = *reinterpret_cast<uint16_t *>(&headerBuffer[peOffset + 22]);
    uint16_t magic = *reinterpret_cast<uint16_t *>(&headerBuffer[peOffset + 24]);

    uint32_t relocDirRVA = 0;
    uint32_t relocDirSize = 0;

    if (magic == 0x10B)
    {
        relocDirRVA = *reinterpret_cast<uint32_t *>(&headerBuffer[peOffset + 160]);
        relocDirSize = *reinterpret_cast<uint32_t *>(&headerBuffer[peOffset + 164]);
    }
    else if (magic == 0x20B)
    {
        relocDirRVA = *reinterpret_cast<uint32_t *>(&headerBuffer[peOffset + 176]);
        relocDirSize = *reinterpret_cast<uint32_t *>(&headerBuffer[peOffset + 180]);
    }

    if ((characteristics & 0x0001) || relocDirRVA == 0 || relocDirSize == 0)
    {
        std::cout << "    -> Relocation Table: Stripped or Empty.\n";
        std::cout << "    -> Note: Executable requires a fixed base memory address to run.\n";
    }
    else
    {
        std::cout << "    -> Relocation Table: Present (Size: " << relocDirSize << " bytes).\n";
        std::cout << "    -> Note: Executable supports Address Space Layout Randomization (ASLR).\n";
    }
}

void Engine::scanExports()
{
    std::cout << "Parsing Export Address Table...\n";
    if (detectedOS != "Windows")
        return;

    auto *dosHeader = reinterpret_cast<IMAGE_DOS_HEADER *>(headerBuffer.data());
    uint32_t peOffset = dosHeader->e_lfanew;
    uint16_t magic = *reinterpret_cast<uint16_t *>(&headerBuffer[peOffset + 24]);

    uint32_t exportDirRVA = 0;

    if (magic == 0x10B)
    {
        exportDirRVA = *reinterpret_cast<uint32_t *>(&headerBuffer[peOffset + 120]);
    }
    else if (magic == 0x20B)
    {
        exportDirRVA = *reinterpret_cast<uint32_t *>(&headerBuffer[peOffset + 136]);
    }

    if (exportDirRVA == 0)
    {
        std::cout << "    -> Export Directory: Not Present.\n";
        return;
    }

    uint32_t exportOffset = rvaToOffset(exportDirRVA);
    if (exportOffset == 0 || exportOffset + 40 > headerBuffer.size())
        return;

    uint32_t numberOfFunctions = *reinterpret_cast<uint32_t *>(&headerBuffer[exportOffset + 20]);
    uint32_t numberOfNames = *reinterpret_cast<uint32_t *>(&headerBuffer[exportOffset + 24]);
    uint32_t addressOfNamesRVA = *reinterpret_cast<uint32_t *>(&headerBuffer[exportOffset + 32]);

    std::cout << "    -> Export Directory: Present.\n";
    std::cout << "    -> Total Exported Functions: " << numberOfFunctions << "\n";
    std::cout << "    -> Named Exports: " << numberOfNames << "\n";

    // Safely extract the first few function names if they exist
    if (numberOfNames > 0 && numberOfNames < 10000)
    {
        uint32_t namesOffset = rvaToOffset(addressOfNamesRVA);
        if (namesOffset > 0 && namesOffset < headerBuffer.size())
        {
            uint32_t displayCount = (numberOfNames > 5) ? 5 : numberOfNames;
            for (uint32_t i = 0; i < displayCount; ++i)
            {
                uint32_t nameRVA = *reinterpret_cast<uint32_t *>(&headerBuffer[namesOffset + (i * 4)]);
                uint32_t nameStringOffset = rvaToOffset(nameRVA);
                if (nameStringOffset > 0 && nameStringOffset < headerBuffer.size())
                {
                    std::string funcName(&headerBuffer[nameStringOffset]);
                    std::cout << "       - " << funcName << "\n";
                }
            }
            if (numberOfNames > 5)
            {
                std::cout << "       ... (and " << (numberOfNames - 5) << " more)\n";
            }
        }
    }
}