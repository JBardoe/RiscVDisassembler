#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <functional>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include "disassembler/RiscvTypes.hpp"

namespace Disassembler {

class SymbolTable {
   public:
    SymbolTable() = default;

    void addSymbol(std::string name, uint32_t addr, uint32_t size,
                   SymbolType type, Assembly::SymbolBinding binding,
                   std::string sectionName);
    std::optional<std::reference_wrapper<const Symbol>> getSymbolName(
        std::string name);
    std::vector<Symbol> getSymbolAddr(uint32_t addr);
    std::vector<Symbol> getSymbolSection(std::string sectionName);

    std::string toString();

   private:
    std::vector<Symbol> table;
    std::unordered_map<std::string, int> nameLookup;
    std::unordered_map<uint32_t, std::vector<int>> addrLookup;
    std::unordered_map<std::string, std::vector<int>> sectionLookup;
};

}  // namespace Disassembler

#endif