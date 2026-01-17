#include <memory>

#include "utils/AssemblyFile.hpp"

namespace Disassembler {
std::unique_ptr<Instruction> parseInstruction(
    uint32_t raw);  // Decodes an instruction from its 32-bit binary
std::unique_ptr<AssemblyFile> disassemble(
    const std::string& filepath);  // Main disassembler method
}  // namespace Disassembler