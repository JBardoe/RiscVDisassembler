#include "utils/AssemblySection.hpp"

const std::string& TextSection::toString() {
    if (this->printOut != "") return this->printOut;

    this->printOut = ".text\n";

    for (auto& instr : this->instructions) {
        printOut += "\t" + instr->toString() + "\n";
    }

    return this->printOut;
}