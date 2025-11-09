#ifndef ELFFILE_H
#define ELFFILE_H

#include <elfio/elfio.hpp>

class ELFFile {
   public:
    ELFFile(std::unique_ptr<ELFIO::elfio> file);

    std::unique_ptr<ELFIO::elfio> getFile();

   private:
    std::unique_ptr<ELFIO::elfio> file;
};

#endif