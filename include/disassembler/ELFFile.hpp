#ifndef ELFFILE_H
#define ELFFILE_H

#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "disassembler/ELFTypes.hpp"

namespace ELFParser {
class ELFSection;
class ELFSegment;

class ELFFile {
   public:
    std::unique_ptr<std::ifstream> stream;
    bool isLittleEndian;
    ELFFile(std::unique_ptr<std::ifstream> st, std::unique_ptr<ELFHeader> hd)
        : stream(std::move(st)),
          isLittleEndian(hd->identifiers[5] == 1),
          header(std::move(hd)) {};

    void parseSections();
    void parseSegments();

    const std::unordered_map<std::string, std::unique_ptr<ELFSection>>&
    getSections() {
        return sections;
    };
    const std::vector<std::unique_ptr<ELFSegment>>& getSegments() {
        return segments;
    };
    const std::string& getSectionName(int index) {
        if (sectionIndexes.find(index) == sectionIndexes.end()) {
            return "";
        }
        return sectionIndexes[index];
    }

   private:
    std::unique_ptr<ELFHeader> header;
    std::unordered_map<std::string, std::unique_ptr<ELFSection>> sections;
    std::unordered_map<int, std::string> sectionIndexes;
    std::vector<std::unique_ptr<ELFSegment>> segments;
};

class ELFSection {
   public:
    std::unique_ptr<SectionHeader> header;
    ELFSection(ELFFile* file, std::unique_ptr<SectionHeader> hdr)
        : header(std::move(hdr)), data(nullptr), file(file), loaded(false) {};

    ~ELFSection() {
        if (data) delete[] data;
    }

    const char* getData();

   private:
    char* data;
    ELFFile* file;
    bool loaded;
};

class ELFSegment {
   public:
    std::unique_ptr<SegmentHeader> header;
    ELFSegment(ELFFile* file, std::unique_ptr<SegmentHeader> hdr)
        : header(std::move(hdr)), data(nullptr), file(file), loaded(false) {};

    ~ELFSegment() {
        if (data) delete[] data;
    }

    char* getData();

   private:
    char* data;
    ELFFile* file;
    bool loaded;
};
}  // namespace ELFParser

#endif