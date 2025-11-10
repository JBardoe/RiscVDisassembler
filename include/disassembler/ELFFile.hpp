#ifndef ELFFILE_H
#define ELFFILE_H

#include <string>

#include "disassembler/ELFTypes.hpp"

class ELFFile {
   public:
    ELFFile();

    void setHeader(const ELFHeader& hd) { header = hd; };

   private:
    ELFHeader& header;
};

#endif