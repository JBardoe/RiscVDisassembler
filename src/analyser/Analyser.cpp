#include "analyser/Analyser.hpp"

#include <array>
#include <memory>
#include <ranges>
#include <unordered_map>
#include <vector>

#include "analyser/AnalysisTypes.hpp"
#include "translator/ArmInstruction.hpp"
#include "translator/ArmSection.hpp"
#include "utils/AnalysisException.hpp"

namespace Analyser {

void analyse(Translator::ArmFile& file) {
    size_t instructionCount = 0;
    std::unique_ptr<std::unordered_map<InstructionClass, int>> mix =
        std::make_unique<std::unordered_map<InstructionClass, int>>();
    std::array<int, 2> forwardBackwardBranches{};

    auto& sections = file.getSections();

    auto textIt = sections.find(".text");

    if (textIt == sections.end()) {
        throw AnalysisException("ARM file is missing a .text section.");
    }

    auto* textSection =
        dynamic_cast<Translator::TextSection*>(textIt->second.get());

    if (!textSection) {
        throw AnalysisException(".text section is not well-formed.");
    }

    auto& instructions = textSection->getInstructions();

    for (auto& block : instructions) {
        for (auto& instr : block) {
            auto instrAnalysis = instr->getAnalysis();

            if (instrAnalysis.type == InstructionClass::ENTRY) continue;

            instructionCount++;
            (*mix)[instrAnalysis.type]++;

            if (instrAnalysis.type == InstructionClass::BRANCH) {
                forwardBackwardBranches[static_cast<int>(
                    instrAnalysis.branchDirection)]++;
            }
        }
    }

    // TODO hazards

    auto report = std::make_unique<Analysis>(instructionCount, std::move(mix),
                                             forwardBackwardBranches);
    file.setAnalysis(std::move(report));
}
}  // namespace Analyser