#include "disassembler/AssemblySection.hpp"

namespace Disassembler {

const std::string& TextSection::toString() {
    if (this->printOut != "") return this->printOut;

    this->printOut = ".text\n";

    for (auto entry : entryPoints) {
        printOut += to_string(entry.second) + " " + entry.first + "\n";
    }

    for (auto& instr : this->instructions) {
        printOut += instr->toString() + "\n";
    }

    return this->printOut;
}

void DataSection::addVariable(std::string name, uint32_t addr, uint32_t val,
                              uint32_t size) {
    vars[name] = Variable(name, addr, val, size);
    addrLookup[addr].push_back(name);
}

std::optional<std::reference_wrapper<const Variable>> DataSection::getVar(
    std::string name) {
    if (auto it = vars.find(name); it != vars.end()) {
        return (*it).second;
    }
    return std::nullopt;
}

// When looknig up by address there is no way to discern between variable at
// that address is wanted so we just return the first one found
std::optional<std::reference_wrapper<const Variable>> DataSection::getVar(
    uint32_t addr) {
    if (auto it = addrLookup.find(addr); it != addrLookup.end()) {
        return vars[(*it).second[0]];
    }
    return std::nullopt;
}

const std::string& DataSection::toString() {
    if (printOut != "") return printOut;

    printOut = ".data\n";

    if (addrLookup.empty()) {
        printOut += "<Empty>\n\n";
        return printOut;
    }

    for (auto& addrList : addrLookup) {
        if (addrList.second.empty()) continue;  // Should not trigger

        if (addrList.second.size() == 1) {
            printOut += "\t" + to_string(vars[addrList.second[0]]) + "\n";
            continue;
        }

        for (auto var : addrList.second) {
            printOut += "\t" + vars[var].name + ":\n";
        }
        printOut +=
            "\t\t" + std::to_string(vars[addrList.second[0]].val) + "\n";
    }

    printOut += "\n";

    return printOut;
}

}  // namespace Disassembler