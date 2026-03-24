#ifndef DISASSEMBLER_HPP
#define DISASSEMBLER_HPP

#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>

#include "disassembler/RiscvFile.hpp"
#include "disassembler/RiscvInstruction.hpp"
#include "utils/AssemblySection.hpp"

namespace Disassembler {
/**
 * Decodes an instruction from its 32-bit binary
 *
 * @param raw 32-bit binary representation of the instruction from the file
 *
 * @return Parsed version of the instruction
 */
std::unique_ptr<RiscvInstruction> parseInstruction(uint32_t raw);

/**
 * Decodes the symbol table and adds it to the file
 *
 * @param asmFile Pointer to the assembly file under construction
 * @param elffile Pointer to the raw ELF file
 * @param sections Map of the sections in the ELF file
 */
void disassembleSymbolTable(
    const std::unique_ptr<RiscvFile>& asmFile,
    const std::unique_ptr<ELFParser::ELFFile>& elffile,
    const std::unordered_map<std::string,
                             std::unique_ptr<ELFParser::ELFSection>>& sections);

/**
 * Decodes the .data section if it is present
 *
 * @param asmFile Pointer to the assembly file under construction
 * @param dataSection Pointer to the raw .data section
 * @param isLittleEndian
 *
 * @return Created assembly .data section
 */
std::shared_ptr<Assembly::DataSection> disassembleDataSection(
    const std::unique_ptr<RiscvFile>& asmFile,
    const std::unique_ptr<ELFParser::ELFSection>& dataSection,
    bool isLittleEndian);

/**
 * Decodes the .bss section if it is present
 *
 * @param asmFile Pointer to the assembly file under construction
 * @param bssSection Pointer to the raw .bss section
 *
 * @return Created assembly .bss section
 */
std::shared_ptr<Assembly::BSSSection> disassembleBSSSection(
    const std::unique_ptr<RiscvFile>& asmFile,
    const std::unique_ptr<ELFParser::ELFSection>& bssSection);

/**
 * Decodes the .text section (has to be present)
 *
 * @param asmFile Pointer to the assembly file under construction
 * @param textSection Pointer to the raw .text section
 * @param gpAddress Address of the global pointer
 * @param isLittleEndian
 *
 * @return Created assembly .text section
 */
std::shared_ptr<TextSection> disassembleTextSection(
    const std::unique_ptr<RiscvFile>& asmFile,
    const std::unique_ptr<ELFParser::ELFSection>& textSection,
    uint32_t gpAddress, bool isLittleEndian);

/**
 * Main disassembler method
 *
 * @param filepath Path to the file
 *
 * @return Created assembly file
 */
std::unique_ptr<RiscvFile> disassemble(const std::string& filepath);
}  // namespace Disassembler

#endif