#include "disassembler/Disassembler.hpp"

#include <format>
#include <vector>

#include "disassembler/DisassemblerTypes.hpp"
#include "disassembler/ELFFile.hpp"
#include "disassembler/Instruction.hpp"
#include "disassembler/Parser.hpp"
#include "utils/DisassemblyException.hpp"

using namespace std;

namespace Disassembler {

unique_ptr<Instruction> parseInstruction(uint32_t raw) {
    // Get opcode
    if (opcodeMap.find(raw & 0x7F) == opcodeMap.end()) {
        throw DisassemblyException("Unknown Opcode: " +
                                   format("{:02X}", (raw & 0x7F)));
    }

    Opcode opcode = opcodeMap.at(raw & 0x7F);

    switch (opcode) {
        // R-Type
        case Opcode::R_TYPE:
            return make_unique<RInstruction>(opcode, raw);

        // I-Types
        case Opcode::LOAD:
        case Opcode::IMM_INSTR:
        case Opcode::JALR:
        case Opcode::ENV_TYPE:
            return make_unique<IInstruction>(opcode, raw);

        // S-Type
        case Opcode::S_TYPE:
            return make_unique<SInstruction>(opcode, raw);

        // B-Type
        case Opcode::B_TYPE:
            return make_unique<BInstruction>(opcode, raw);

        // U-Type
        case Opcode::AUIPC:
        case Opcode::LUI:
            return make_unique<UInstruction>(opcode, raw);

        // J-Type
        default:
            return make_unique<JInstruction>(opcode, raw);
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
        Deal with endianness
    */

    /*
        Extract instructions
    */

    return nullptr;
}
}  // namespace Disassembler