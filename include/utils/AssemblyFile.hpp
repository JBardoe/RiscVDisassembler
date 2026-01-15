#ifndef ASSEMBLYFILE_H
#define ASSEMBLYFILE_H

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "disassembler/Instruction.hpp"
#include "utils/AssemblySection.hpp"

class AssemblyFile {
   public:
    AssemblyFile(
        std::vector<std::unique_ptr<Disassembler::Instruction>> textSection)
        : sections({}), printOut("") {};

    void addSection(std::unique_ptr<AssemblySection> section) {
        sections.insert({section->getName(), std::move(section)});
    }

    const std::string& toString();

   private:
    std::unordered_map<const std::string&, std::unique_ptr<AssemblySection>>
        sections;
    std::string printOut;
};

#endif
