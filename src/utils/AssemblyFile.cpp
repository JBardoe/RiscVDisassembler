#include "utils/AssemblyFile.hpp"

const std::string& AssemblyFile::toString() {
    if (this->printOut != "") return this->printOut;

    for (auto& sec : this->sections) {
        printOut += sec.second->toString();
        printOut += "\n";
    }

    return this->printOut;
}

void AssemblyFile::addSymbol(std::string name,
                             std::unique_ptr<RISCV::Symbol> sym,
                             uint32_t type) {
    if (symbolTable.find(name) == symbolTable.end() || type == 2) {
        symbolTable.insert({name, std::move(sym)});
    }
}