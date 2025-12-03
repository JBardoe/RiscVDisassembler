#include <fstream>
#include <memory>
#include <string>

#include "disassembler/ELFFile.hpp"

std::unique_ptr<ELFFile> parseFile(const std::string& filepath);

ELFHeader* parseHeader(std::ifstream& filestream);