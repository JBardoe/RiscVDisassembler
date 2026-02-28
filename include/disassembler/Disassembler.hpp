#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>

#include "disassembler/RiscvFile.hpp"
#include "disassembler/RiscvInstruction.hpp"
#include "utils/AssemblySection.hpp"

namespace Disassembler {
std::unique_ptr<RiscvInstruction> parseInstruction(
    uint32_t raw);  // Decodes an instruction from its 32-bit binary
void disassembleSymbolTable(
    const std::unique_ptr<RiscvFile>& asmFile,
    const std::unique_ptr<ELFParser::ELFFile>& elffile,
    const std::unordered_map<std::string,
                             std::unique_ptr<ELFParser::ELFSection>>& sections);
std::unique_ptr<Assembly::DataSection> disassembleDataSection(
    const std::unique_ptr<RiscvFile>& asmFile,
    const std::unique_ptr<ELFParser::ELFSection>& dataSection,
    bool isLittleEndian);

std::unique_ptr<TextSection> disassembleTextSection(
    const std::unique_ptr<RiscvFile>& asmFile,
    const std::unique_ptr<ELFParser::ELFSection>& textSection,
    uint32_t gpAddress, bool isLittleEndian);
std::unique_ptr<RiscvFile> disassemble(
    const std::string& filepath);  // Main disassembler method
}  // namespace Disassembler