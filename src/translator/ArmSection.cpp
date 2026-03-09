#include "translator/ArmSection.hpp"

#include <ranges>

namespace Translator {

const std::string& TextSection::toString() {
    if (this->printOut != "") return this->printOut;

    this->printOut = ".text\n";

    // Collapse 2D array of instructions into 1D
    auto joined = std::ranges::join_view(instructions);

    std::vector<std::unique_ptr<ArmInstruction>> all(
        std::make_move_iterator(joined.begin()),
        std::make_move_iterator(joined.end()));

    for (auto entry : entryPoints) {
        printOut += to_string(entry.second) + " " + entry.first + "\n";
    }

    for (auto& instr : all) {
        printOut += instr->toString() + "\n";
    }

    return this->printOut;
}
}  // namespace Translator