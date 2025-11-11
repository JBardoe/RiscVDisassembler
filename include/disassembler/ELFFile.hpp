#ifndef ELFFILE_H
#define ELFFILE_H

#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "disassembler/ELFTypes.hpp"

class ELFSection;
class ELFSegment;

class ELFFile {
   public:
    bool isLittleEndian;
    std::unique_ptr<std::ifstream> stream;
    ELFFile(std::unique_ptr<std::ifstream> st, const ELFHeader& hd)
        : stream(std::move(st)), header(hd) {};

    void parseSections();
    void parseSegments();

    const ELFHeader& getHeader() { return header; }
    std::unordered_map<std::string, std::unique_ptr<ELFSection>>&
    getSections() {
        return sections;
    };
    std::vector<std::unique_ptr<ELFSegment>>& getSegments() {
        return segments;
    };

   private:
    const ELFHeader& header;
    std::unordered_map<std::string, std::unique_ptr<ELFSection>> sections;
    std::vector<std::unique_ptr<ELFSegment>> segments;
};

class ELFSection {
   public:
    ELFSection(ELFFile* file, const SectionHeader& hdr)
        : header(hdr), data(nullptr), file(file), loaded(false) {};

    const char* getData();
    const SectionHeader& getHeader() { return header; }

   private:
    const SectionHeader& header;
    char* data;
    ELFFile* file;
    bool loaded;
};

class ELFSegment {
   public:
    ELFSegment(ELFFile* file, const SegmentHeader& hdr)
        : header(hdr), data(nullptr), file(file), loaded(false) {};

    char* getData();
    const SegmentHeader& getHeader() { return header; }

   private:
    const SegmentHeader& header;
    char* data;
    ELFFile* file;
    bool loaded;
};

#endif