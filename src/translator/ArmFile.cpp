#include "translator/ArmFile.hpp"

#include <ranges>
#include <vector>

#include "disassembler/RiscvSection.hpp"

namespace Translator {

ArmFile::ArmFile(const std::unique_ptr<Disassembler::RiscvFile>& riscFile)
    : printOut("") {  // TODO implement
    auto riscSections = riscFile->getSections();
    auto riscTextSection = dynamic_cast<Disassembler::TextSection*>(
        riscSections.find(".text")->second.get());

    auto riscTextInstructions = riscTextSection->getInstructions();

    // TODO deal with jalr when instructions inflate
    // TODO eliminate gp and tp

    std::vector<std::vector<ArmInstruction>> instructions;

    for (auto& instr : riscTextInstructions) {
        instructions.push_back(instr->toArm());
    }

    sections.insert({".text", std::make_shared<TextSection>(
                                  instructions | std::views::join,
                                  riscTextSection->getEntryPoints())});

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
}  // namespace Translator

// TODO implement
