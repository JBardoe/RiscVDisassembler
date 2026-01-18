#include <fstream>
#include <memory>
#include <string>

#include "disassembler/ELFFile.hpp"

namespace ELFParser {

/**
 * Opens and parses an ELF file
 *
 * @param filepath relative or absolute path to the file
 * @return pointer to the ELF file object
 */
std::unique_ptr<ELFFile> parseFile(const std::string& filepath);

/**
 * Parses the header of an ELF file
 *
 * @param filestream open stream to the file
 * @return pointer to the ELF header object
 */
std::unique_ptr<ELFHeader> parseHeader(std::ifstream& filestream);
}  // namespace ELFParser