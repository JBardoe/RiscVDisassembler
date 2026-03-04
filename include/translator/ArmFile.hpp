#ifndef ARMFILE_HPP
#define ARMFILE_HPP
#include <memory>
#include <string>
#include <unordered_map>

#include "disassembler/RiscvFile.hpp"
#include "translator/ArmSection.hpp"
#include "utils/AssemblySection.hpp"

namespace Translator {

class ArmFile {  // TODO implement
   public:
    ArmFile(const std::unique_ptr<Disassembler::RiscvFile>& riscFile);

    const std::string& toString();

   private:
    std::string printOut;
    std::unordered_map<std::string, std::shared_ptr<Assembly::AssemblySection>>
        sections;
};

}  // namespace Translator

#endif