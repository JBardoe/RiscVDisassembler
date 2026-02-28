#include "utils/AssemblyTypes.hpp"

namespace Assembly {
std::string to_string(Variable v) {
    return v.name + ":\t" + std::string(variableTypes[v.size - 1]) + " " +
           std::to_string(v.val);
}

std::string to_string(SymbolBinding b) {
    switch (b) {
        case SymbolBinding::LOCAL:
            return ".local";
        case SymbolBinding::GLOBAL:
            return ".globl";
        case SymbolBinding::WEAK:
            return ".weak";
        case SymbolBinding::LOOS:
            return ".loos";
        case SymbolBinding::HIOS:
            return ".hios";
        case SymbolBinding::LOPROC:
            return ".loproc";
        case SymbolBinding::HIPROC:
            return ".hiproc";
    }
    return "<unknown_binding>";
}
}  // namespace Assembly