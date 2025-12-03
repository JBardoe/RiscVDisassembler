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
    ELFFile(std::unique_ptr<std::ifstream> st, ELFHeader* hd)
        : stream(std::move(st)), header(hd) {};

    ~ELFFile(){
        if(header) delete header;
    }

    void parseSections();
    void parseSegments();

    ELFHeader* getHeader() { return header; }
    std::unordered_map<std::string, std::unique_ptr<ELFSection>>&
    getSections() {
        return sections;
    };
    std::vector<std::unique_ptr<ELFSegment>>& getSegments() {
        return segments;
    };

   private:
    ELFHeader* header;
    std::unordered_map<std::string, std::unique_ptr<ELFSection>> sections;
    std::vector<std::unique_ptr<ELFSegment>> segments;
};

class ELFSection {
   public:
    ELFSection(ELFFile* file, SectionHeader hdr)
        : header(hdr), data(nullptr), file(file), loaded(false) {};
    
    ~ELFSection(){
        if(data) delete[] data;
    }
    
    const char* getData();
    const SectionHeader& getHeader() { return header; }

   private:
    SectionHeader header;
    char* data;
    ELFFile* file;
    bool loaded;
};

class ELFSegment {
   public:
    ELFSegment(ELFFile* file, SegmentHeader hdr)
        : header(hdr), data(nullptr), file(file), loaded(false) {};

    ~ELFSegment(){
        if(data) delete[] data;
    }

    char* getData();
    const SegmentHeader& getHeader() { return header; }

   private:
    SegmentHeader header;
    char* data;
    ELFFile* file;
    bool loaded;
};

#endif