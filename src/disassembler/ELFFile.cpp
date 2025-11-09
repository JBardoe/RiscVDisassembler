#include "disassembler/ELFFile.hpp"

ELFFile::ELFFile(std::unique_ptr<ELFIO::elfio> file) : file(std::move(file)) {}