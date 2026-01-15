#include <memory>

#include "utils/AssemblyFile.hpp"

namespace Disassembler {
std::unique_ptr<AssemblyFile> disassemble(const std::string& filepath);
}