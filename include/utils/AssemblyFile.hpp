#ifndef ASSEMBLYFILE_H
#define ASSEMBLYFILE_H

#include <memory>
#include <string>
#include <vector>

#include "disassembler/Instruction.hpp"

class AssemblyFile {
   public:
    AssemblyFile(
        std::vector<std::unique_ptr<Disassembler::Instruction>> textSection)
        : textSection(std::move(textSection)), printOut("") {};

    const std::string& toString();

   private:
    std::vector<std::unique_ptr<Disassembler::Instruction>> textSection;
    std::string printOut;
};

#endif
