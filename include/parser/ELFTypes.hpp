#ifndef ELFTYPES_H
#define ELFTYPES_H

#include <cstdint>

namespace ELFParser {

/**
 * Header of an ELF file
 */
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

/**
 * Header of a section from an ELF file
 */
typedef struct SectionHeader {
    uint32_t name;
    uint32_t type;
    uint32_t flags;
    uint32_t address;  // Virtual address at execution
    uint32_t offset;   // Offset to the section in the file
    uint32_t size;
    uint32_t sh_link;  // Link to another section
    uint32_t info;     // Additional information
    uint32_t addressAlignment;
    uint32_t entrySize;  // Entry size if the section holds a table
} SectionHeader;

/**
 * Header of a segment from an ELF file
 */
typedef struct SegmentHeader {
    uint32_t type;
    uint32_t offset;    // Offset to the section in the file
    uint32_t vaddr;     // Virtual address
    uint32_t paddr;     // Physical address (when relevant)
    uint32_t fileSize;  // Size of the file image
    uint32_t memSize;   // Size of the memory image
    uint32_t flags;
    uint32_t align;
} SegmentHeader;

/**
 * Single entry in an ELF file's symbol table
 */
typedef struct SymbolTableEntry {
    uint32_t name;        // Index into symbol string table of the name
    uint32_t value;       // Value of the symbol
    uint32_t size;        // Size of the symbol
    unsigned char info;   // Type and binding attributes
    unsigned char other;  // Visibility
    uint16_t shndx;       // Index of the section header containing the symbol
} SymbolTableEntry;

}  // namespace ELFParser
#endif