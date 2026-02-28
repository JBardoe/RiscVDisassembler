#include "disassembler/RiscvFile.hpp"

namespace Disassembler {
const std::string& RiscvFile::toString() {
    if (this->printOut != "") return this->printOut;

    printOut += symbolTable.toString();

    for (auto& sec : this->sections) {
        printOut += sec.second->toString();
        printOut += "\n";
    }

    return this->printOut;
}

void RiscvFile::addSymbol(std::string name, uint32_t addr, uint32_t size,
                          SymbolType type, Assembly::SymbolBinding binding,
                          std::string sectionName) {
    symbolTable.addSymbol(name, addr, size, type, binding, sectionName);
}

std::optional<std::reference_wrapper<const Symbol>> RiscvFile::getSymbolName(
    std::string name) {
    return symbolTable.getSymbolName(name);
}
std::vector<Symbol> RiscvFile::getSymbolAddr(uint32_t addr) {
    return symbolTable.getSymbolAddr(addr);
}
std::vector<Symbol> RiscvFile::getSymbolSection(std::string sectionName) {
    return symbolTable.getSymbolSection(sectionName);
}

std::string RiscvFile::printSymbolTable() { return symbolTable.toString(); }

}  // namespace Disassembler