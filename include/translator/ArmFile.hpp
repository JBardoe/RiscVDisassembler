#ifndef ARMFILE_HPP
#define ARMFILE_HPP
#include <memory>
#include <string>
#include <unordered_map>

#include "analyser/AnalysisTypes.hpp"
#include "disassembler/RiscvFile.hpp"
#include "translator/ArmSection.hpp"
#include "utils/AssemblySection.hpp"

namespace Translator {

/**
 * Represents a translated ARM file
 */
class ArmFile {
   public:
    ArmFile() = default;
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

    void setAnalysis(std::unique_ptr<Analyser::Analysis> analysisReport) {
        this->analysisReport = std::move(analysisReport);
    }

    const std::unique_ptr<Analyser::Analysis>& getAnalysis() const {
        return analysisReport;
    }

    const std::unordered_map<std::string,
                             std::shared_ptr<Assembly::AssemblySection>>&
    getSections() const {
        return sections;
    }

   private:
    std::string printOut;  // String readout of the file
    std::unordered_map<std::string, std::shared_ptr<Assembly::AssemblySection>>
        sections;  // Map of the sections in the file

    std::unique_ptr<Analyser::Analysis> analysisReport;
};

}  // namespace Translator

#endif