#ifndef ARMFILE_HPP
#define ARMFILE_HPP
#include <memory>
#include <string>
#include <unordered_map>

#include "disassembler/RiscvFile.hpp"
#include "translator/ArmSection.hpp"
#include "utils/AssemblySection.hpp"

namespace Translator {

/**
 * Represents a translated ARM file
 */
class ArmFile {
   public:
    ArmFile(const std::unique_ptr<Disassembler::RiscvFile>& riscFile);

    /**
     * Gives a string readout of the ARM file
     *
     * @return string equivalent of the file
     */
    const std::string& toString();

    /**
     * Write the string readout to a file
     *
     * @param fileName Name of the file to write to
     */
    void writeToFile(std::string fileName);

   private:
    std::string printOut;  // String readout of the file
    std::unordered_map<std::string, std::shared_ptr<Assembly::AssemblySection>>
        sections;  // Map of the sections in the file
};

}  // namespace Translator

#endif