#include <vector>

#include "translator/ArmTypes.hpp"

namespace Analyser {

typedef struct Analysis {
} Analysis;

enum class InstructionClass {
    ALU,
    LOAD,
    STORE,
    BRANCH,
    JUMP,
    OTHER,
};

typedef struct InstructionAnalysis {
    std::vector<Translator::Register> reads;
    Translator::Register write;

    InstructionClass type;

    int imm;
} InstructionAnalysis;

}  // namespace Analyser