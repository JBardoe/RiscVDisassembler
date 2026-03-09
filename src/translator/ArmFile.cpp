#include "translator/ArmFile.hpp"

#include <fstream>
#include <vector>

#include "disassembler/RiscvSection.hpp"

namespace Translator {

ArmFile::ArmFile(const std::unique_ptr<Disassembler::RiscvFile>& riscFile)
    : printOut("") {
    // Translate text instructions into ARM
    auto riscSections = riscFile->getSections();
    auto riscTextSection = dynamic_cast<Disassembler::TextSection*>(
        riscSections.find(".text")->second.get());

    const std::vector<std::unique_ptr<Disassembler::RiscvInstruction>>&
        riscTextInstructions = riscTextSection->getInstructions();

    std::vector<std::vector<std::unique_ptr<ArmInstruction>>>
        instructions;  // Initially 2D as some instructions expand into two

    for (auto& instr : riscTextInstructions) {
        instructions.push_back(std::move(instr->toArm()));
    }

    sections.insert({".text", std::make_shared<TextSection>(
                                  std::move(instructions),
                                  riscTextSection->getEntryPoints(),
                                  riscTextSection->getBasicBlocks())});

    // Transfer .data and .bss over directly if present (no changes are
    // required)
    if (auto it = riscSections.find(".data"); it != riscSections.end()) {
        sections.insert({".data", it->second});
    }

    if (auto it = riscSections.find(".bss"); it != riscSections.end()) {
        sections.insert({".bss", it->second});
    }
}

const std::string& ArmFile::toString() {
    if (this->printOut != "") return this->printOut;

    for (auto& sec : this->sections) {
        printOut += sec.second->toString();
        printOut += "\n";
    }

    return this->printOut;
}

void ArmFile::writeToFile(std::string fileName) {
    std::ofstream outFile("data/output/ARM/" + fileName + ".s");

    outFile << toString();

    outFile.close();
}

}  // namespace Translator
