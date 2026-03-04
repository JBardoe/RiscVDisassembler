#ifndef RISCVFILE_H
#define RISCVFILE_H

#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "disassembler/RiscvSection.hpp"
#include "disassembler/RiscvTypes.hpp"
#include "disassembler/SymbolTable.hpp"
#include "utils/AssemblySection.hpp"

namespace Disassembler {
/**
 * Represents a disassembled assembly file
 */
class RiscvFile {
   public:
    RiscvFile(FileType type)
        : printOut(""), symbolTable(SymbolTable()), type(type) {};

    /**
     * Adds a section that has been decoded from the file
     *
     * @param name name of the section
     * @param section pointer to the section object
     */
    void addSection(std::string name,
                    std::shared_ptr<Assembly::AssemblySection> section) {
        sections.insert({name, section});
    }

    const std::unordered_map<std::string,
                             std::shared_ptr<Assembly::AssemblySection>>&
    getSections() {
        return sections;
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
                   SymbolType type, Assembly::SymbolBinding binding,
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

    FileType getFileType() { return type; }

   private:
    std::unordered_map<std::string, std::shared_ptr<Assembly::AssemblySection>>
        sections;             // Map of section names to sections
    std::string printOut;     // String readout of the file
    SymbolTable symbolTable;  // Map of symbol names to symbols
    FileType type;
};
}  // namespace Disassembler
#endif
