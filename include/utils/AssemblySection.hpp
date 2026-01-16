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
    virtual const std::string& toString() { return this->printOut; };
    const std::string& getName() { return this->name; }

   protected:
    std::string name;
    std::string printOut;
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