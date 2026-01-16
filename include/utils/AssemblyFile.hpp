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
    AssemblyFile() : printOut("") {};

    void addSection(std::string name,
                    std::unique_ptr<AssemblySection> section) {
        sections.insert({name, std::move(section)});
    }

    const std::string& toString();

   private:
    std::unordered_map<std::string, std::unique_ptr<AssemblySection>> sections;
    std::string printOut;
};

#endif
