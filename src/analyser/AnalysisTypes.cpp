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

std::string to_string(HazardType t) {
    switch (t) {
        case HazardType::RAW:
            return "RAW";
        case HazardType::WAW:
            return "WAW";
        case HazardType::WAR:
            return "WAR";
        default:
            return "<unknown_hazard_type>";
    }
}

std::string to_string(HazardSeverity s) {
    switch (s) {
        case HazardSeverity::HIGH:
            return "HIGH";
        case HazardSeverity::MEDIUM:
            return "MEDIUM";
        case HazardSeverity::LOW:
            return "LOW";
        default:
            return "<unknown_severity>";
    }
}

HazardSeverity getSeverity(Translator::Operator instr, int diff) {
    switch (instr) {
        case Translator::Operator::add:
        case Translator::Operator::sub:
        case Translator::Operator::eor:
        case Translator::Operator::orr:
        case Translator::Operator::And:
        case Translator::Operator::cset:
        case Translator::Operator::adr:
        case Translator::Operator::movz:
            return (diff == 1) ? HazardSeverity::MEDIUM : HazardSeverity::LOW;

        case Translator::Operator::lsl:
        case Translator::Operator::lsr:
        case Translator::Operator::asr:
            if (diff == 1) return HazardSeverity::HIGH;
            if (diff == 2) return HazardSeverity::MEDIUM;
            return HazardSeverity::LOW;

        case Translator::Operator::ldrsw:
        case Translator::Operator::ldrsb:
        case Translator::Operator::ldrsh:
        case Translator::Operator::ldrb:
        case Translator::Operator::ldrh:
            if (diff < 3) return HazardSeverity::HIGH;
            if (diff < 5) return HazardSeverity::MEDIUM;
            return HazardSeverity::LOW;
        default:
            return HazardSeverity::LOW;
    }
}

std::string to_string(Hazard h) {
    return to_string(h.type) + " hazard between instructions " +
           std::to_string(h.first) + " and " + std::to_string(h.second) +
           " -> Severity: " + to_string(h.severity) + "\n";
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
            "\nBranch Density:\n\t- Forward Branches: " +
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

    if (!a.hazards->empty()) {
        ret += "\nThe following blocks have data dependencies:\n";
    }

    for (auto& block : *(a.hazards)) {
        ret +=
            "\t- Block ending on line " + std::to_string(block.first) + ":\n";
        for (auto& hazard : block.second) {
            ret += "\t\t- " + to_string(hazard);
        }
    }

    if (!a.excessSpillageBlocks.empty()) {
        ret += "\nThe following blocks have excessive register spillage:\n";
        for (auto& block : a.excessSpillageBlocks) {
            ret += "\t- Block ending on line " + std::to_string(block) + "\n";
        }
    }
    return ret;
}

}  // namespace Analyser