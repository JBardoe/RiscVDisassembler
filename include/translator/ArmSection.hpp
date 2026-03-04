#ifndef ARMSECTION_HPP
#define ARMSECTION_HPP

#include <memory>
#include <string>
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
    TextSection(std::vector<std::unique_ptr<ArmInstruction>> instructions,
                std::vector<std::pair<std::string, Assembly::SymbolBinding>>
                    entryPoints)
        : ArmSection(".text"),
          instructions(std::move(instructions)),
          entryPoints(entryPoints) {}

    /**
     * toString method to print the text section in assembly form
     *
     * @return string readout version of the section
     */
    const std::string& toString() override;

   private:
    std::vector<std::unique_ptr<ArmInstruction>>
        instructions;  // Vector of the instructions in the section
    std::vector<std::pair<std::string, Assembly::SymbolBinding>>
        entryPoints;  // Entry points into the .text section
};
}  // namespace Translator

#endif