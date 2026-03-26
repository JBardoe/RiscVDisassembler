#ifndef ANALYSISTYPES_HPP
#define ANALYSISTYPES_HPP

#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "translator/ArmTypes.hpp"
#include "utils/AssemblyTypes.hpp"

namespace Analyser {

enum class InstructionClass : int {
    ALU,
    LOAD,
    STORE,
    BRANCH,
    JUMP,
    ENTRY,
    OTHER,
};

std::string to_string(InstructionClass i);

typedef struct InstructionAnalysis {
    std::vector<Translator::Register> reads;
    Translator::Register write;

    InstructionClass type;
    Assembly::BranchDirection branchDirection;

    int imm;
} InstructionAnalysis;

enum class HazardType : int {
    RAW,
    WAW,
    WAR,
};

std::string to_string(HazardType t);

enum class HazardSeverity : int {
    HIGH,
    MEDIUM,
    LOW,
};

std::string to_string(HazardSeverity s);

typedef struct Hazard {
    HazardType type;

    size_t first;
    size_t second;

    HazardSeverity severity;
} Hazard;

std::string to_string(Hazard h);

typedef struct Analysis {
    size_t instructionCount;
    std::unique_ptr<std::unordered_map<InstructionClass, int>> instructionMix;
    std::array<int, 2> forwardBackwardBranches;
    std::map<int, std::vector<Hazard>> hazards;
} Analysis;

std::string to_string(const Analysis& a);
}  // namespace Analyser

#endif