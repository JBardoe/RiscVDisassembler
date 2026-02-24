#ifndef ELFFILE_H
#define ELFFILE_H

#include <cstdint>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "parser/ELFTypes.hpp"

namespace ELFParser {
class ELFSection;
class ELFSegment;

/**
 * Represents a raw ELF file before it is disassembled
 */
class ELFFile {
   public:
    std::unique_ptr<std::ifstream>
        stream;  // Open stream to the file to allow data to be loaded as needed
    bool isLittleEndian;  // Whether the file is in little (true) or big (false)
                          // endian

    ELFFile(std::unique_ptr<std::ifstream> st, std::unique_ptr<ELFHeader> hd)
        : stream(std::move(st)),
          isLittleEndian(hd->identifiers[5] == 1),
          header(std::move(hd)) {};

    void parseSections();  // Extracts the sections
    void parseSegments();  // Extracts the segments

    const std::unordered_map<std::string, std::unique_ptr<ELFSection>>&
    getSections() {
        return sections;
    };
    const std::vector<std::unique_ptr<ELFSegment>>& getSegments() {
        return segments;
    }

    const std::unique_ptr<ELFHeader>& getHeader() { return header; }

    /**
     * Matches a section index to the name of the section
     *
     * @param index index of the section in the section header table
     * @return name of the associated section (empty string if not found)
     */
    std::string getSectionName(int index) {
        if (sectionIndexes.find(index) == sectionIndexes.end()) {
            return "";
        }
        return sectionIndexes[index];
    }

   private:
    std::unique_ptr<ELFHeader> header;  // Main file header
    std::unordered_map<std::string, std::unique_ptr<ELFSection>>
        sections;  // Mapping of section names to the section
    std::unordered_map<uint16_t, std::string>
        sectionIndexes;  // Mapping of the section index to its name
    std::vector<std::unique_ptr<ELFSegment>> segments;  // Vector of the
                                                        // segments
};

/**
 * Represents a raw section of an ELF file before disassembly
 */
class ELFSection {
   public:
    std::unique_ptr<SectionHeader> header;  // Header of the section

    ELFSection(ELFFile* file, std::unique_ptr<SectionHeader> hdr)
        : header(std::move(hdr)), data(nullptr), file(file), loaded(false) {};

    ~ELFSection() {
        if (data) delete[] data;
    }

    /**
     * Load and return the full binary data of the section
     *
     * @return binary section data from the file
     */
    const char* getData();

   private:
    char* data;     // Section data from the file
    ELFFile* file;  // Pointer to the file
    bool loaded;    // Whether the data has been loaded from the file
};

/**
 * Represents a raw segment of an ELF file before disassembly
 */
class ELFSegment {
   public:
    std::unique_ptr<SegmentHeader> header;  // Header of the segment

    ELFSegment(ELFFile* file, std::unique_ptr<SegmentHeader> hdr)
        : header(std::move(hdr)), data(nullptr), file(file), loaded(false) {};

    ~ELFSegment() {
        if (data) delete[] data;
    }

    /**
     * Load and return the full binary data of the segment
     *
     * @return binary segment data from the file
     */
    char* getData();

   private:
    char* data;     // Section data from the file
    ELFFile* file;  // Pointer to the file
    bool loaded;    // Whether the data has been loaded from the file
};
}  // namespace ELFParser

#endif