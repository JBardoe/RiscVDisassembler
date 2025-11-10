#ifndef ELFFILE_H
#define ELFFILE_H

#include <string>

#include "disassembler/ELFTypes.hpp"

class ELFFile {
   public:
    bool isLittleEndian;
    ELFFile();

    void setHeader(const ELFHeader& hd) { header = hd; };
    const ELFHeader& getHeader() { return header; }

   private:
    ELFHeader& header;
};

class ELFSection {
   public:
    ELFSection(const char* data, const int size) : data(data), size(size) {};

   private:
    const char* data;
    const int size;
};

#endif