#include "analyser/AnalysisTypes.hpp"

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
        case InstructionClass::OTHER:
            return "OTHER";
        default:
            return "<unknown_instruction_type>";
    }
}

}  // namespace Analyser