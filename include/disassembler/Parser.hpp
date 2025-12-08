#include <fstream>
#include <memory>
#include <string>

#include "disassembler/ELFFile.hpp"

namespace ELFParser {

std::unique_ptr<ELFFile> parseFile(const std::string& filepath);

std::unique_ptr<ELFHeader> parseHeader(std::ifstream& filestream);
}  // namespace ELFParser