#ifndef ANALYSISTYPES_HPP
#define ANALYSISTYPES_HPP

#include <string>
#include <vector>

#include "translator/ArmTypes.hpp"

namespace Analyser {

typedef struct Analysis {
    std::unique_ptr<std::unordered_map<InstructionClass, int>> instructionMix;
} Analysis;

enum class InstructionClass {
    ALU,
    LOAD,
    STORE,
    BRANCH,
    JUMP,
    OTHER,
};

std::string to_string(InstructionClass i);

typedef struct InstructionAnalysis {
    std::vector<Translator::Register> reads;
    Translator::Register write;

    InstructionClass type;

    int imm;
} InstructionAnalysis;

}  // namespace Analyser

#endif