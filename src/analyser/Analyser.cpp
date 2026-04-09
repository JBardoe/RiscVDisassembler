#include "analyser/Analyser.hpp"

#include <array>
#include <iterator>
#include <memory>
#include <ranges>
#include <unordered_map>
#include <utility>
#include <vector>

#include "analyser/AnalysisTypes.hpp"
#include "translator/ArmInstruction.hpp"
#include "translator/ArmSection.hpp"
#include "utils/AnalysisException.hpp"

namespace Analyser {

void analyse(Translator::ArmFile& file) {
    size_t instructionCount = 0;
    auto mix = std::make_unique<std::unordered_map<InstructionClass, int>>();
    std::array<int, 2> forwardBackwardBranches{};
    auto hazards = std::make_unique<std::map<int, std::vector<Hazard>>>();

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
            auto& instrAnalysis = instr->getAnalysis();

            if (instrAnalysis.type == InstructionClass::ENTRY) continue;

            instructionCount++;
            (*mix)[instrAnalysis.type]++;

            if (instrAnalysis.type == InstructionClass::BRANCH) {
                forwardBackwardBranches[static_cast<int>(
                    instrAnalysis.branchDirection)]++;
            }
        }
    }

    for (auto block = textSection->getBasicBlocks()->begin();
         block != std::prev(textSection->getBasicBlocks()->end()); block++) {
        auto current = std::next(block);

        std::unordered_map<Translator::Register,
                           std::pair<int, Translator::Operator>>
            lastWrites;
        std::unordered_map<Translator::Register, int> lastReads;

        int instrNum = 0;

        for (int i = block->first + 1; i < current->first; i++) {
            for (auto& instr : instructions[i]) {
                if (instr->instr == Translator::Operator::entry) continue;
                auto& instrAnalysis = instr->getAnalysis();

                // RAW
                for (auto& reg : instrAnalysis.reads) {
                    if (auto it = lastWrites.find(reg);
                        it != lastWrites.end()) {
                        if (HazardSeverity sev = getSeverity(
                                it->second.second, instrNum - it->second.first);
                            sev != HazardSeverity::LOW) {
                            (*hazards)[current->first].push_back(
                                Hazard(HazardType::RAW, it->second.first,
                                       instrNum, sev));
                        }
                    }
                }

                // WAW
                if (auto it = lastWrites.find(instrAnalysis.write);
                    it != lastWrites.end()) {
                    if (HazardSeverity sev = getSeverity(
                            it->second.second, instrNum - it->second.first);
                        sev != HazardSeverity::LOW) {
                        (*hazards)[current->first].push_back(Hazard(
                            HazardType::WAW, it->second.first, instrNum, sev));
                    }
                }

                // WAR
                if (auto it = lastReads.find(instrAnalysis.write);
                    it != lastReads.end()) {
                    if (HazardSeverity sev =
                            getSeverity(instr->instr, instrNum - it->second);
                        sev != HazardSeverity::LOW) {
                        (*hazards)[current->first].push_back(
                            Hazard(HazardType::WAR, it->second, instrNum, sev));
                    }
                }

                lastWrites[instrAnalysis.write] =
                    std::make_pair(instrNum, instr->instr);
                for (auto& reg : instrAnalysis.reads) {
                    lastReads[reg] = instrNum;
                }
                instrNum++;
            }
        }
    }

    auto report = std::make_unique<Analysis>(
        instructionCount, std::move(mix), forwardBackwardBranches,
        std::move(hazards), std::unordered_set<int>{});

    file.setAnalysis(std::move(report));
}
}  // namespace Analyser