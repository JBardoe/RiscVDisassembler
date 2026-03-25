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

    void addVariable(std::string name, uint32_t val, uint32_t size) {
        if (auto it = sections.find(".data"); it != sections.end()) {
            if (auto* data =
                    dynamic_cast<Assembly::DataSection*>(it->second.get())) {
                data->addVariable(name, val, size);
            }
        } else {
            auto dataSection = std::make_unique<Assembly::DataSection>();
            dataSection->addVariable(name, val, size);
            addSection(".data", std::move(dataSection));
        }
    }

    /**
     * Getters for the symbols from the symbol table from the:
     * getSymbolName - Name of the symbol
     * getSymbolAddr - Address of the symbol
     * getSymbolSections - Section name
     */
    std::optional<std::reference_wrapper<const Symbol>> getSymbolName(
        std::string name) const;
    std::vector<Symbol> getSymbolAddr(uint32_t addr) const;
    std::vector<Symbol> getSymbolSection(std::string sectionName) const;

    /**
     * Gives a string readout of the RISC-V file
     *
     * @return string equivalent of the file
     */
    const std::string& toString();

    /**
     * Write the string readout to a file
     *
     * @param fileName Name of the file to write to
     */
    void writeToFile(std::string fileName);

    FileType getFileType() const { return type; }

    const std::unordered_map<std::string,
                             std::shared_ptr<Assembly::AssemblySection>>&
    getSections() const {
        return sections;
    }

   private:
    std::unordered_map<std::string, std::shared_ptr<Assembly::AssemblySection>>
        sections;             // Map of section names to sections
    std::string printOut;     // String readout of the file
    SymbolTable symbolTable;  // Map of symbol names to symbols
    FileType type;            // File type (e.g. executable)
};
}  // namespace Disassembler
#endif
