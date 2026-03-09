#ifndef RISCVSECTION_H
#define RISCVSECTION_H

#include <map>
#include <string>
#include <unordered_set>

#include "disassembler/RiscvInstruction.hpp"
#include "translator/ArmInstruction.hpp"
#include "utils/AssemblySection.hpp"

namespace Disassembler {

/**
 * Represents a section of a RISC-V file
 */
class RiscvSection : public virtual Assembly::AssemblySection {
   public:
    RiscvSection() = default;
    RiscvSection(std::string name) : Assembly::AssemblySection(name) {}

    virtual ~RiscvSection() = default;
};

/**
 * Represents a text section in a RISC-V file
 */
class TextSection : public virtual RiscvSection {
   public:
    TextSection(std::vector<std::unique_ptr<RiscvInstruction>> instructions,
                std::vector<std::pair<std::string, Assembly::SymbolBinding>>
                    entryPoints);

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

    std::shared_ptr<std::map<int, std::unique_ptr<std::unordered_set<int>>>>
    getBasicBlocks() {
        return basicBlocks;
    }

   private:
    std::vector<std::unique_ptr<RiscvInstruction>>
        instructions;  // Vector of the instructions in the section
    std::vector<std::pair<std::string, Assembly::SymbolBinding>>
        entryPoints;  // Vector of entry points in the .text section

    std::shared_ptr<std::map<int, std::unique_ptr<std::unordered_set<int>>>>
        basicBlocks;  // Map of ends of basic blocks to a set of live registers
                      // in that block
};

}  // namespace Disassembler
#endif