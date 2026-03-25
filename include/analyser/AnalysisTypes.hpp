#ifndef ANALYSISTYPES_HPP
#define ANALYSISTYPES_HPP

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "translator/ArmTypes.hpp"
#include "utils/AssemblyTypes.hpp"

namespace Analyser {

enum class InstructionClass {
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

typedef struct Analysis {
    size_t instructionCount;
    std::unique_ptr<std::unordered_map<InstructionClass, int>> instructionMix;
    std::array<int, 2> forwardBackwardBranches;
} Analysis;

std::string to_string(const Analysis& a);
}  // namespace Analyser

#endif