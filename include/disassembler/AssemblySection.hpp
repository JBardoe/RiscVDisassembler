#ifndef ASSEMBLYSECTION_H
#define ASSEMBLYSECTION_H

#include <memory>
#include <string>
#include <vector>

#include "disassembler/Instruction.hpp"

namespace Disassembler {

/**
 * Represents a section of an assembly file
 */
class AssemblySection {
   public:
    AssemblySection() = default;
    AssemblySection(std::string name) : name(name), printOut("") {}

    virtual ~AssemblySection() = default;

    /**
     * Default toString method to print the section
     * Overridden for specific sections
     *
     * @return the empty string
     */
    virtual const std::string& toString() { return this->printOut; };

    const std::string& getName() { return this->name; }

   protected:
    std::string name;      // Name of the section
    std::string printOut;  // String readout of the section
};

/**
 * Represents a text section in an assembly file
 */
class TextSection : public AssemblySection {
   public:
    TextSection(std::vector<std::unique_ptr<Instruction>> instructions)
        : AssemblySection(".text"), instructions(std::move(instructions)) {}

    /**
     * toString method to print the text section in assembly form
     *
     * @return string readout version of the section
     */
    const std::string& toString() override;

   private:
    std::vector<std::unique_ptr<Instruction>>
        instructions;  // Vector of the instructions in the section
};

// class DataSection : public AssemblySection{
//     public:
//     DataSection()

//     const std::string& toString() override;

//     private:

// };

}  // namespace Disassembler
#endif