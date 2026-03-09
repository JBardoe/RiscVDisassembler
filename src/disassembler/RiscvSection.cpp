#include "disassembler/RiscvSection.hpp"

namespace Disassembler {

TextSection::TextSection(
    std::vector<std::unique_ptr<RiscvInstruction>> instructions,
    std::vector<std::pair<std::string, Assembly::SymbolBinding>> entryPoints)
    : RiscvSection(".text"),
      instructions(std::move(instructions)),
      entryPoints(entryPoints) {
    basicBlocks = std::make_shared<
        std::map<int, std::unique_ptr<std::unordered_set<int>>>>();

    basicBlocks->insert({-1, std::make_unique<std::unordered_set<int>>()});

    auto liveRegisters = std::make_unique<std::unordered_set<int>>();

    for (std::size_t i = 0; i < this->instructions.size(); i++) {
        for (auto& reg : this->instructions[i]->getRegistersUsed()) {
            liveRegisters->insert(static_cast<int>(reg));
            registersUsed->insert(static_cast<int>(reg));
        }

        if (this->instructions[i]->op == Opcode::B_TYPE ||
            this->instructions[i]->instr == Operator::jal ||
            this->instructions[i]->instr == Operator::jalr) {
            basicBlocks->insert({i, std::move(liveRegisters)});
            liveRegisters = std::make_unique<std::unordered_set<int>>();
        }
    }

    if (!basicBlocks->count(this->instructions.size() - 1)) {
        basicBlocks->insert(
            {this->instructions.size() - 1, std::move(liveRegisters)});
    }
}

const std::string& TextSection::toString() {
    if (this->printOut != "") return this->printOut;

    this->printOut = ".text\n";

    for (auto entry : entryPoints) {
        printOut += to_string(entry.second) + " " + entry.first + "\n";
    }

    for (auto& instr : this->instructions) {
        printOut += instr->toString() + "\n";
    }

    return this->printOut;
}

}  // namespace Disassembler