#include "disassembler/Disassembler.hpp"

#include <vector>

#include "disassembler/DisassemblerTypes.hpp"
#include "disassembler/ELFFile.hpp"
#include "disassembler/Parser.hpp"
#include "utils/BadFileException.hpp"

using namespace std;

namespace Disassembler {

unique_ptr<Instruction> disassembleIntruction(const RawInstruction& raw) {
    // Get opcode
    bool msb = raw.byte1 & 0x80;

    if (opcodeMap.find(raw.byte1 & 0x7F) == opcodeMap.end()) {
        // TODO throw error
    }

    Opcode opcode = opcodeMap.at(raw.byte1 & 0x7F);

    switch (opcode) {
        case Opcode::LOAD:
            break;
        case Opcode::IMM_INSTR:
            break;
        case Opcode::AUIPC:
            break;
        case Opcode::S_TYPE:
            break;
        case Opcode::R_TYPE:
            break;
        case Opcode::LUI:
            break;
        case Opcode::B_TYPE:
            break;
        case Opcode::JALR:
            break;
        case Opcode::JAL:
            break;
        case Opcode::ENV_TYPE:
            break;
    }
}

unique_ptr<AssemblyFile> disassemble(const string& filepath) {
    unique_ptr<ELFParser::ELFFile> elffile = ELFParser::parseFile(filepath);
    if (!elffile) return nullptr;

    /*
        Checks inc:
            1. Required sections
            2. Sections well formed
            ...
    */

    /*
        Extract instructions
    */

    return nullptr;
}
}  // namespace Disassembler