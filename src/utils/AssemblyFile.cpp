#include "utils/AssemblyFile.hpp"

const std::string& AssemblyFile::toString() {
    if (this->printOut != "") return this->printOut;

    this->printOut = ".text\n";

    for (auto& instr : this->textSection) {
        printOut += "\t" + instr->toString() + "\n";
    }

    return this->printOut;
}
