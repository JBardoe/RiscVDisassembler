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

    const std::vector<std::unique_ptr<Disassembler::RiscvInstruction>>&
        riscTextInstructions = riscTextSection->getInstructions();

    // // TODO deal with jalr when instructions inflate
    // // TODO eliminate gp and tp

    std::vector<std::vector<std::unique_ptr<ArmInstruction>>> instructions;

    for (auto& instr : riscTextInstructions) {
        instructions.push_back(std::move(instr->toArm()));
    }

    auto joined = std::ranges::join_view(instructions);

    std::vector<std::unique_ptr<ArmInstruction>> all(
        std::make_move_iterator(joined.begin()),
        std::make_move_iterator(joined.end()));

    sections.insert(
        {".text", std::make_shared<TextSection>(
                      std::move(all), riscTextSection->getEntryPoints())});

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
