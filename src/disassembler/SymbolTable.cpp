#include "disassembler/SymbolTable.hpp"

namespace Disassembler {

void SymbolTable::addSymbol(std::string name, uint32_t addr, uint32_t size,
                            SymbolType type, SymbolBinding binding,
                            std::string sectionName) {
    table.emplace_back(name, addr, size, type, binding, sectionName);
    nameLookup[name] = table.size() - 1;
    addrLookup[addr].push_back(table.size() - 1);
    sectionLookup[sectionName].push_back(table.size() - 1);
}

std::optional<std::reference_wrapper<const Symbol>> SymbolTable::getSymbolName(
    std::string name) {
    if (auto it = nameLookup.find(name); it != nameLookup.end()) {
        return table[(*it).second];
    }
    return std::nullopt;
}

std::vector<Symbol> SymbolTable::getSymbolAddr(
    uint32_t addr) {  // NOTE: could be optimised
    auto it = addrLookup.find(addr);
    if (it == addrLookup.end()) return {};

    std::vector<Symbol> ret;

    for (auto i : (*it).second) {
        ret.push_back(table[i]);
    }

    return ret;
}

std::vector<Symbol> SymbolTable::getSymbolSection(
    std::string sectionName) {  // NOTE: could be optimised
    auto it = sectionLookup.find(sectionName);
    if (it == sectionLookup.end()) return {};

    std::vector<Symbol> ret;

    for (auto i : (*it).second) {
        ret.push_back(table[i]);
    }

    return ret;
}

std::string SymbolTable::toString() {
    std::string ret = "Symbol Table:\n";

    for (auto sym : table) {
        ret += to_string(sym);
    }

    ret += "\n";

    return ret;
}
}  // namespace Disassembler