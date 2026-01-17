#include <memory>

#include "utils/AssemblyFile.hpp"

namespace Disassembler {
std::unique_ptr<Instruction> parseInstruction(uint32_t raw);
std::unique_ptr<AssemblyFile> disassemble(const std::string& filepath);
}  // namespace Disassembler