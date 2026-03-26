#include "translator/ArmSection.hpp"

namespace Translator {

const std::string& TextSection::toString() {
    if (this->printOut != "") return this->printOut;

    this->printOut = ".text\n";

    for (auto& entry : entryPoints) {
        printOut += to_string(entry.second) + " " + entry.first + "\n";
    }

    for (auto& block : instructions) {
        for (auto& instr : block) {
            printOut += instr->toString() + "\n";
        }
    }

    return this->printOut;
}
}  // namespace Translator