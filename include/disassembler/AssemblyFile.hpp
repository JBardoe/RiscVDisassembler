#ifndef ASSEMBLYFILE_H
#define ASSEMBLYFILE_H

#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "disassembler/AssemblySection.hpp"
#include "disassembler/Instruction.hpp"
#include "disassembler/RiscvTypes.hpp"

namespace Disassembler {
/**
 * Represents a disassembled assembly file
 */
class AssemblyFile {
   public:
    AssemblyFile() : printOut("") {};

    /**
     * Adds a section that has been decoded from the file
     *
     * @param name name of the section
     * @param section pointer to the section object
     */
    void addSection(std::string name,
                    std::unique_ptr<AssemblySection> section) {
        sections.insert({name, std::move(section)});
    }

    /**
     * Adds a decoded symbol to the symbol table
     *
     * @param name name of the symbol
     * @param sym pointer to the symbol struct
     * @param type type of the associated section
     */
    void addSymbol(std::string name, std::unique_ptr<Disassembler::Symbol> sym,
                   uint32_t type);

    const std::unordered_map<std::string,
                             std::unique_ptr<Disassembler::Symbol>>&
    getSymbolTable() {
        return this->symbolTable;
    }

    /**
     * Gives a string readout of the assembly file
     *
     * @return string equivalent of the file
     */
    const std::string& toString();

   private:
    std::unordered_map<std::string, std::unique_ptr<AssemblySection>>
        sections;  // Map of section names to sections
    std::unordered_map<std::string, std::unique_ptr<Disassembler::Symbol>>
        symbolTable;       // Map of symbol names to symbols
    std::string printOut;  // String readout of the file
};
}  // namespace Disassembler
#endif
