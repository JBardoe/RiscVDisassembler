#ifndef TRANSLATOR_HPP
#define TRANSLATOR_HPP

#include <memory>

#include "disassembler/RiscvFile.hpp"
#include "translator/ArmFile.hpp"

namespace Translator {

std::unique_ptr<ArmFile> translate(
    std::unique_ptr<Disassembler::RiscvFile> riscvFile);
}
// TODO translate relqtive jumps and branches into offsets
// TODO eliminate gp and tp

#endif