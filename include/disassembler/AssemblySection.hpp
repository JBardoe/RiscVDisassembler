#ifndef ASSEMBLYSECTION_H
#define ASSEMBLYSECTION_H

#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <utility>
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

    void addEntryPointsOffset(const std::vector<Symbol>& entries);

   private:
    std::vector<std::unique_ptr<Instruction>>
        instructions;  // Vector of the instructions in the section
    std::vector<std::pair<std::string, SymbolBinding>> entryPoints;
};

class DataSection : public AssemblySection {
   public:
    DataSection() = default;

    void addVariable(std::string name, uint32_t addr, uint32_t val,
                     uint32_t size);

    std::optional<std::reference_wrapper<const Variable>> getVar(
        std::string name);
    std::optional<std::reference_wrapper<const Variable>> getVar(uint32_t addr);

    const std::string& toString() override;

   private:
    std::unordered_map<std::string, Variable>
        vars;  // TODO check if name lookup will be needed
    std::unordered_map<uint32_t, std::vector<std::string>> addrLookup;
};

}  // namespace Disassembler
#endif