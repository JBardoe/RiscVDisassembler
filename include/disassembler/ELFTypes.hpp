#ifndef ELFTYPES_H
#define ELFTYPES_H

#include <cstdint>

typedef struct ELFHeader {
    unsigned char identifiers[16];
    uint16_t fileType;
    uint16_t architecture;
    uint32_t elfVersion;
    uint32_t entryAddress;
    uint32_t phoff;  // Offset of the program header table
    uint32_t shoff;  // Offset of the section header table
    uint32_t flags;  // Processor specific flags
    uint16_t headerSize;
    uint16_t phentrySize;  // Size of an entry in the program header table
    uint16_t phnum;        // Number of entries in the program header table
    uint16_t shentrySize;  // Size of an entry in the section header table
    uint16_t shnum;        // Number of entries in the section header table
    uint16_t shstrndx;  // Index in the section header table of the section name
                        // string table
} ELFHeader;

#endif