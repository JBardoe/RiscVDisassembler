#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <functional>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include "disassembler/RiscvTypes.hpp"

namespace Disassembler {

/**
 * Represents the symbol table in the RISC-V file
 */
class SymbolTable {
   public:
    SymbolTable() = default;

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
     * Prints the symbol table
     *
     * @return string version of the table
     */
    std::string toString();

   private:
    std::vector<Symbol> table;  // Symbol table
    std::unordered_map<std::string, int>
        nameLookup;  // For getting symbols by symbol name
    std::unordered_map<uint32_t, std::vector<int>>
        addrLookup;  // For getting symbols by address
    std::unordered_map<std::string, std::vector<int>>
        sectionLookup;  // For getting symbols by section name
};

}  // namespace Disassembler

#endif