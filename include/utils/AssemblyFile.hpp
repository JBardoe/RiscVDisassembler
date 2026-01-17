#ifndef ASSEMBLYFILE_H
#define ASSEMBLYFILE_H

#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "disassembler/Instruction.hpp"
#include "disassembler/RiscvTypes.hpp"
#include "utils/AssemblySection.hpp"

class AssemblyFile {
   public:
    AssemblyFile() : printOut("") {};

    void addSection(std::string name,
                    std::unique_ptr<AssemblySection> section) {
        sections.insert({name, std::move(section)});
    }

    void addSymbol(std::string name, std::unique_ptr<RISCV::Symbol> sym,
                   uint32_t type);

    const std::unordered_map<std::string, std::unique_ptr<RISCV::Symbol>>&
    getSymbolTable() {
        return this->symbolTable;
    }

    const std::string& toString();

   private:
    std::unordered_map<std::string, std::unique_ptr<AssemblySection>> sections;
    std::unordered_map<std::string, std::unique_ptr<RISCV::Symbol>> symbolTable;
    std::string printOut;
};

#endif
