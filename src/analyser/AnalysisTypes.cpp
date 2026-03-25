#include "analyser/AnalysisTypes.hpp"

#include <cmath>
#include <format>

namespace Analyser {

std::string to_string(InstructionClass i) {
    switch (i) {
        case InstructionClass::ALU:
            return "ALU";
        case InstructionClass::LOAD:
            return "LOAD";
        case InstructionClass::STORE:
            return "STORE";
        case InstructionClass::BRANCH:
            return "BRANCH";
        case InstructionClass::JUMP:
            return "JUMP";
        case InstructionClass::ENTRY:
            return "ENTRY";
        case InstructionClass::OTHER:
            return "OTHER";
        default:
            return "<unknown_instruction_type>";
    }
}

std::string to_string(const Analysis& a) {
    std::string ret = "Instruction Mix:\n";

    for (auto& instrType : *(a.instructionMix)) {
        ret += "\t- " + to_string(instrType.first) + ": " +
               std::format(
                   "{:.2f}",
                   std::round(static_cast<double>(instrType.second) /
                              static_cast<double>(a.instructionCount) * 10000) /
                       100) +
               "%\n";
    }

    auto branchIt = a.instructionMix->find(InstructionClass::BRANCH);

    if (branchIt != a.instructionMix->end()) {
        ret +=
            "Branch Density:\n\t- Forward Branches: " +
            std::to_string(a.forwardBackwardBranches[0]) + " | " +
            std::format(
                "{:.2f}",
                std::round(static_cast<double>(a.forwardBackwardBranches[0]) /
                           static_cast<double>(branchIt->second) * 10000) /
                    100) +
            "%\n" + "\t- Backwards Branches: " +
            std::to_string(a.forwardBackwardBranches[1]) + " | " +
            std::format(
                "{:.2f}",
                std::round(static_cast<double>(a.forwardBackwardBranches[1]) /
                           static_cast<double>(branchIt->second) * 10000) /
                    100) +
            "%\n";
    } else {
        ret += "Branch Density:\n\tNO CONDITIONAL BRANCHES\n";
    }

    return ret;
}

}  // namespace Analyser