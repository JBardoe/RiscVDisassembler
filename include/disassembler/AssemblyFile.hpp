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
#include "disassembler/SymbolTable.hpp"

namespace Disassembler {
/**
 * Represents a disassembled assembly file
 */
class AssemblyFile {
   public:
    AssemblyFile() : printOut(""), symbolTable(SymbolTable()) {};

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
     * @param addr address of the symbol
     * @param size size of the symbol
     * @param type type of the symbol
     * @param binding binding of the symbol
     * @param sectionName name of the section the symbol is in
     */
    void addSymbol(std::string name, uint32_t addr, uint32_t size,
                   SymbolType type, SymbolBinding binding,
                   std::string sectionName);

    std::optional<std::reference_wrapper<const Symbol>> getSymbolName(
        std::string name);
    std::vector<Symbol> getSymbolAddr(uint32_t addr);
    std::vector<Symbol> getSymbolSection(std::string sectionName);

    std::string printSymbolTable();

    /**
     * Gives a string readout of the assembly file
     *
     * @return string equivalent of the file
     */
    const std::string& toString();

   private:
    std::unordered_map<std::string, std::unique_ptr<AssemblySection>>
        sections;             // Map of section names to sections
    std::string printOut;     // String readout of the file
    SymbolTable symbolTable;  // Map of symbol names to symbols
};
}  // namespace Disassembler
#endif
