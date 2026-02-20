#include "disassembler/AssemblyFile.hpp"

namespace Disassembler {
const std::string& AssemblyFile::toString() {
    if (this->printOut != "") return this->printOut;

    for (auto& sec : this->sections) {
        printOut += sec.second->toString();
        printOut += "\n";
    }

    return this->printOut;
}

void AssemblyFile::addSymbol(std::string name, uint32_t addr, uint32_t size,
                             SymbolType type, SymbolBinding binding,
                             std::string sectionName) {
    symbolTable.addSymbol(name, addr, size, type, binding, sectionName);
}

std::optional<std::reference_wrapper<const Symbol>> AssemblyFile::getSymbolName(
    std::string name) {
    return symbolTable.getSymbolName(name);
}
std::vector<Symbol> AssemblyFile::getSymbolAddr(uint32_t addr) {
    return symbolTable.getSymbolAddr(addr);
}
std::vector<Symbol> AssemblyFile::getSymbolSection(std::string sectionName) {
    return symbolTable.getSymbolSection(sectionName);
}

std::string AssemblyFile::printSymbolTable() { return symbolTable.toString(); }

}  // namespace Disassembler