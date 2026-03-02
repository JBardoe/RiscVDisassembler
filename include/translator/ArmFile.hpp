#ifndef ARMFILE_HPP
#define ARMFILE_HPP
#include <memory>
#include <string>

#include "disassembler/RiscvFile.hpp"
namespace Translator {

class ArmFile {  // TODO implement
   public:
    ArmFile(const std::unique_ptr<Disassembler::RiscvFile>& riscFile);

    const std::string& toString();

   private:
    std::string printOut;
};

}  // namespace Translator

#endif