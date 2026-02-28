#ifndef ARMFILE_HPP
#define ARMFILE_HPP
#include <memory>

#include "disassembler/RiscvFile.hpp"
namespace Translator {

class ArmFile {  // TODO implement
   public:
    ArmFile(const std::unique_ptr<Disassembler::RiscvFile>& riscFile);
};

}  // namespace Translator

#endif