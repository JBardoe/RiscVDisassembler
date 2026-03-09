#ifndef ASSEMBLYSECTION_HPP
#define ASSEMBLYSECTION_HPP

#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "utils/AssemblyTypes.hpp"

namespace Assembly {

/**
 * Virtual class representing a generic section in an assembly file
 */
class AssemblySection {
   public:
    AssemblySection() = default;
    AssemblySection(std::string name) : name(name), printOut("") {}

    virtual ~AssemblySection() = default;

    /**
     * Virtual toString method to print the section in assembly form
     *
     * @return string readout version of the section
     */
    virtual const std::string& toString() { return this->printOut; }

    const std::string& getName() { return this->name; }

   protected:
    std::string name;      // Name of the section
    std::string printOut;  // String representation of the file
};

/**
 * Represents a .data section in an assembly file
 */
class DataSection : public virtual AssemblySection {
   public:
    DataSection() = default;

    /**
     * Adds a variable to the .data section
     *
     * @param name Name of the variable
     * @param addr Address of the variable
     * @param val Initial value of the variable
     * @param size Size of the variabled in bytes
     */
    void addVariable(std::string name, uint32_t addr, uint32_t val,
                     uint32_t size);

    void addVariable(std::string name, uint32_t val, uint32_t size);

    // TODO check if can be removed
    std::optional<std::reference_wrapper<const Variable>> getVar(
        std::string name);
    std::optional<std::reference_wrapper<const Variable>> getVar(uint32_t addr);

    bool empty() { return addrLookup.empty(); }

    /**
     * toString method to print the .data section in assembly form
     *
     * @return string readout version of the section
     */
    const std::string& toString() override;

   protected:
    std::unordered_map<std::string, Variable>
        vars;  // TODO check if name lookup will be needed
    std::unordered_map<uint32_t, std::vector<std::string>>
        addrLookup;  // TODO consider just storing the first one found since the
                     // others cannot be emitted
};

/**
 * Represents a .bss section in an assembly file
 */
class BSSSection : public DataSection {
   public:
    BSSSection() : DataSection() {};

    /**
     * Adds a variable to the .bss section
     *
     * @param name Name of the variable
     * @param addr Address of the variable
     * @param size Size of the variabled in bytes
     */
    void addVariable(std::string name, uint32_t addr, uint32_t size) {
        DataSection::addVariable(name, addr, 0, size);
    }

    /**
     * toString method to print the .bss section in assembly form
     *
     * @return string readout version of the section
     */
    const std::string& toString() override;
};

}  // namespace Assembly

#endif