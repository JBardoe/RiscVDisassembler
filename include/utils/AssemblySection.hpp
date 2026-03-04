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

class AssemblySection {
   public:
    AssemblySection() = default;
    AssemblySection(std::string name) : name(name), printOut("") {}

    virtual ~AssemblySection() = default;

    virtual const std::string& toString() { return this->printOut; }

    const std::string& getName() { return this->name; }

   protected:
    std::string name;
    std::string printOut;
};

class DataSection : public virtual AssemblySection {
   public:
    DataSection() = default;

    void addVariable(std::string name, uint32_t addr, uint32_t val,
                     uint32_t size);

    // TODO check if can be removed
    std::optional<std::reference_wrapper<const Variable>> getVar(
        std::string name);
    std::optional<std::reference_wrapper<const Variable>> getVar(uint32_t addr);

    bool empty() { return addrLookup.empty(); }

    const std::string& toString() override;

   protected:
    std::unordered_map<std::string, Variable>
        vars;  // TODO check if name lookup will be needed
    std::unordered_map<uint32_t, std::vector<std::string>>
        addrLookup;  // TODO consider just storing the first one found since the
                     // others cannot be emitted
};

class BSSSection : public DataSection {
   public:
    BSSSection() : DataSection() {};

    void addVariable(std::string name, uint32_t addr, uint32_t size) {
        DataSection::addVariable(name, addr, 0, size);
    }

    const std::string& toString() override;
};

}  // namespace Assembly

#endif