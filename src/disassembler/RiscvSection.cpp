#include "disassembler/RiscvSection.hpp"

namespace Disassembler {

TextSection::TextSection(
    std::vector<std::unique_ptr<RiscvInstruction>> instructions,
    std::vector<std::pair<std::string, Assembly::SymbolBinding>> entryPoints)
    : RiscvSection(".text"),
      instructions(std::move(instructions)),
      entryPoints(entryPoints) {
    registersUsed = std::make_shared<std::unordered_set<int>>();

    // Initialise the basic blocks map with a -1 block to fall back to as the
    // start of the file
    basicBlocks = std::make_shared<
        std::map<int, std::unique_ptr<std::unordered_map<int, int>>>>();

    basicBlocks->insert({-1, std::make_unique<std::unordered_map<int, int>>()});

    // Temp tracker of registers live in the current block
    auto liveRegisters = std::make_unique<std::unordered_map<int, int>>();

    // For each instruction
    for (std::size_t i = 0; i < this->instructions.size(); i++) {
        // Add the registers used in the instruction to the current block and to
        // the file's tracker
        for (auto& reg : this->instructions[i]->getRegistersUsed()) {
            (*liveRegisters)[static_cast<int>(reg)]++;
            registersUsed->insert(static_cast<int>(reg));
        }

        // If at the end of a basic block, mark it in the map
        if (this->instructions[i]->op == Opcode::B_TYPE ||
            this->instructions[i]->instr == Operator::jal ||
            this->instructions[i]->instr == Operator::jalr ||
            this->instructions[i]->instr == Operator::ecall ||
            this->instructions[i]->instr == Operator::ebreak) {
            basicBlocks->insert({i, std::move(liveRegisters)});
            liveRegisters = std::make_unique<std::unordered_map<int, int>>();
        }
    }

    // If the last instruction did not indicate the end of a basic block, add
    // the final block
    if (!basicBlocks->count(this->instructions.size() - 1)) {
        basicBlocks->insert(
            {this->instructions.size() - 1, std::move(liveRegisters)});
    }
}

const std::string& TextSection::toString() {
    if (this->printOut != "") return this->printOut;

    this->printOut = ".text\n";

    for (auto& entry : entryPoints) {
        printOut += to_string(entry.second) + " " + entry.first + "\n";
    }

    for (auto& instr : this->instructions) {
        printOut += instr->toString() + "\n";
    }

    return this->printOut;
}

}  // namespace Disassembler