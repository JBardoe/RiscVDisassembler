#ifndef ASSEMBLYSECTION_H
#define ASSEMBLYSECTION_H

#include <memory>
#include <string>
#include <vector>

#include "disassembler/Instruction.hpp"

class AssemblySection {
   public:
    AssemblySection() = default;
    AssemblySection(std::string name) : name(name), printOut("") {}
    virtual ~AssemblySection() = default;
    virtual const std::string& toString();
    const std::string& getName() { return this->name; }

   protected:
    std::string printOut;
    std::string name;
};

class TextSection : public AssemblySection {
   public:
    TextSection(
        std::vector<std::unique_ptr<Disassembler::Instruction>> instructions)
        : AssemblySection(".text"), instructions(std::move(instructions)) {}
    const std::string& toString() override;

   private:
    std::vector<std::unique_ptr<Disassembler::Instruction>> instructions;
};

#endif