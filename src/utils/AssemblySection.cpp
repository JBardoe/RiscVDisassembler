#include "utils/AssemblySection.hpp"

namespace Assembly {

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

// When looking up by address there is no way to discern between variable at
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
            printOut += "\t" + var + ":\n";
        }
        printOut +=
            "\t\t" + std::to_string(vars[addrList.second[0]].val) + "\n";
    }

    printOut += "\n";

    return printOut;
}

const std::string& BSSSection::toString() {
    if (printOut != "") return printOut;

    printOut = ".bss\n";

    if (addrLookup.empty()) {
        printOut += "<Empty>\n\n";
        return printOut;
    }

    for (auto& addrList : addrLookup) {
        if (addrList.second.empty()) continue;  // Should not trigger

        if (addrList.second.size() == 1) {
            auto var = vars[addrList.second[0]];
            printOut +=
                "\t" + var.name + " .space " + std::to_string(var.size) + "\n";
            continue;
        }

        for (auto var : addrList.second) {
            printOut += "\t" + var + ":\n";
        }
        printOut += "\t\t .space " +
                    std::to_string(vars[addrList.second[0]].size) + "\n";
    }

    printOut += "\n";

    return printOut;
}

void DataSection::addVariable(std::string name, uint32_t val, uint32_t size) {
    uint32_t i = 0;
    while (getVar(i) != std::nullopt) {
        i++;
    }
    addVariable(name, i, val, size);
}

}  // namespace Assembly