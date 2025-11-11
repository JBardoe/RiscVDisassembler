#ifndef ELFFILE_H
#define ELFFILE_H

#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "disassembler/ELFTypes.hpp"

class ELFFile {
   public:
    bool isLittleEndian;
    std::unique_ptr<std::ifstream> stream;
    ELFFile(std::unique_ptr<std::ifstream> st, const ELFHeader& hd)
        : stream(std::move(st)), header(hd) {};

    const ELFHeader& getHeader() { return header; }

    void parseSections();
    void parseSegments();

   private:
    const ELFHeader& header;
    std::unordered_map<std::string, std::unique_ptr<ELFSection>> sections;
    std::vector<std::unique_ptr<ELFSegment>> segments;
};

class ELFSection {
   public:
    ELFSection(ELFFile* file, const SectionHeader& hdr)
        : data(nullptr), file(file), loaded(false), header(hdr) {};

    const char* getData();

   private:
    const SectionHeader& header;
    char* data;
    ELFFile* file;
    bool loaded;
};

class ELFSegment {
   public:
    ELFSegment(ELFFile* file, const SegmentHeader& hdr)
        : data(nullptr), file(file), loaded(false), header(hdr) {};

    char* getData();

   private:
    const SegmentHeader& header;
    char* data;
    ELFFile* file;
    bool loaded;
};

#endif