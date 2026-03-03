#include "translator/ArmFile.hpp"

namespace Translator {

ArmFile::ArmFile(const std::unique_ptr<Disassembler::RiscvFile>& riscFile)
    : printOut("") {  // TODO implement
    // TODO translate relqtive jumps and branches into offsets
    // TODO eliminate gp and tp
}

const std::string& ArmFile::toString() {  // TODO implement
    if (this->printOut != "") return this->printOut;

    for (auto& sec : this->sections) {
        printOut += sec.second->toString();
        printOut += "\n";
    }

    return this->printOut;
}
}  // namespace Translator

// TODO implement
