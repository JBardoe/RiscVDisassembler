#include "translator/ArmFile.hpp"

namespace Translator {

ArmFile::ArmFile(const std::unique_ptr<Disassembler::RiscvFile>& riscFile)
    : printOut("") {  // TODO implement
    // TODO translate relqtive jumps and branches into offsets
    // TODO eliminate gp and tp
}

const std::string& ArmFile::toString() {  // TODO implement
    if (printOut != "") return printOut;

    return "";
}
}  // namespace Translator

// TODO implement
