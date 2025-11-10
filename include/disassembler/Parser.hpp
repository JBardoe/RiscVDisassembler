#include <memory>
#include <string>

#include "disassembler/ELFFile.hpp"

std::unique_ptr<ELFFile> parseFile(const std::string& filepath);

const ELFHeader& parseHeader(const ifstream& filestream);