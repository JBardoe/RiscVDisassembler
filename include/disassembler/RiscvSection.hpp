#ifndef RISCVSECTION_H
#define RISCVSECTION_H

#include <string>

#include "disassembler/RiscvInstruction.hpp"
#include "translator/ArmInstruction.hpp"
#include "utils/AssemblySection.hpp"

namespace Disassembler {

/**
 * Represents a section of an assembly file
 */
class RiscvSection : public virtual Assembly::AssemblySection {
   public:
    RiscvSection() = default;
    RiscvSection(std::string name) : Assembly::AssemblySection(name) {}

    virtual ~RiscvSection() = default;
};

/**
 * Represents a text section in an assembly file
 */
class TextSection : public virtual RiscvSection {
   public:
    TextSection(std::vector<std::unique_ptr<RiscvInstruction>> instructions,
                std::vector<std::pair<std::string, Assembly::SymbolBinding>>
                    entryPoints)
        : RiscvSection(".text"),
          instructions(std::move(instructions)),
          entryPoints(entryPoints) {}

    /**
     * toString method to print the text section in assembly form
     *
     * @return string readout version of the section
     */
    const std::string& toString() override;

    const std::vector<std::unique_ptr<RiscvInstruction>>& getInstructions() {
        return instructions;
    }

    std::vector<std::pair<std::string, Assembly::SymbolBinding>>
    getEntryPoints() {
        return entryPoints;
    }

   private:
    std::vector<std::unique_ptr<RiscvInstruction>>
        instructions;  // Vector of the instructions in the section
    std::vector<std::pair<std::string, Assembly::SymbolBinding>> entryPoints;
};

}  // namespace Disassembler
#endif