#include "analyser/Analyser.hpp"

#include <array>
#include <memory>
#include <ranges>
#include <unordered_map>

#include "analyser/AnalysisTypes.hpp"
#include "translator/ArmSection.hpp"
#include "utils/AnalysisException.hpp"

namespace Analyser {

void analyse(Translator::ArmFile& file) {
    std::unique_ptr<std::unordered_map<InstructionClass, int>> mix;
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

    auto& instructions = std::ranges::join_view(textSection->getInstructions());

    for (auto& instr : instructions) {
        auto instrAnalysis = instr->getAnalysis();
        (*mix)[instrAnalysis.type]++;

        if (instrAnalysis.type == InstructionClass::BRANCH) {
            forwardBackwardBranches[static_cast<int>(
                instrAnalysis.branchDirection)]++;
        }
    }

    auto report =
        std::make_unique<Analysis>(std::move(mix), forwardBackwardBranches);
    file.setAnalysis(std::move(report));
}
}  // namespace Analyser