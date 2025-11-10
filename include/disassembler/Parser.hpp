#include <string>

#include "disassembler/ELFFile.hpp"

std::unique_ptr<ELFFile> parseFile(const std::string& filepath);

void parseHeader(const ifstream& filestream, const ELFFile& file);