#ifndef ARMSECTION_HPP
#define ARMSECTION_HPP

#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "translator/ArmInstruction.hpp"
#include "utils/AssemblySection.hpp"
#include "utils/AssemblyTypes.hpp"

namespace Translator {

/**
 * Represents a section in an ARM file
 */
class ArmSection : public virtual Assembly::AssemblySection {
   public:
    ArmSection() = default;
    ArmSection(std::string name) : Assembly::AssemblySection(name) {}

    virtual ~ArmSection() = default;
};

/**
 * Represents a text section in an assembly file
 */
class TextSection : public virtual ArmSection {
   public:
    TextSection(
        std::vector<std::vector<std::unique_ptr<ArmInstruction>>> instructions,
        std::vector<std::pair<std::string, Assembly::SymbolBinding>>
            entryPoints,
        std::shared_ptr<
            std::map<int, std::unique_ptr<std::unordered_map<int, int>>>>
            basicBlocks,
        std::shared_ptr<std::unordered_set<int>> registersUsed)
        : ArmSection(".text"),
          instructions(std::move(instructions)),
          entryPoints(entryPoints),
          basicBlocks(std::move(basicBlocks)),
          registersUsed(registersUsed) {}

    /**
     * toString method to print the text section in assembly form
     *
     * @return string readout version of the section
     */
    const std::string& toString() override;

    std::shared_ptr<
        std::map<int, std::unique_ptr<std::unordered_map<int, int>>>>
    getBasicBlocks() const {
        return basicBlocks;
    }

    std::shared_ptr<std::unordered_set<int>> getRegistersUsed() const {
        return registersUsed;
    }

    const std::vector<std::vector<std::unique_ptr<ArmInstruction>>>&
    getInstructions() const {
        return instructions;
    }

   private:
    std::vector<std::vector<std::unique_ptr<ArmInstruction>>>
        instructions;  // Vector of the instructions in the section
    std::vector<std::pair<std::string, Assembly::SymbolBinding>>
        entryPoints;  // Entry points into the .text section

    std::shared_ptr<
        std::map<int, std::unique_ptr<std::unordered_map<int, int>>>>
        basicBlocks;  // Map of ends of basic blocks to a set of live registers
                      // in that block

    std::shared_ptr<std::unordered_set<int>>
        registersUsed;  // Registers used in a file
};
}  // namespace Translator

#endif