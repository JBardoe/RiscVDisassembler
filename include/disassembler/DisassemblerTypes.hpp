#ifndef DISASSEMBLERTYPES_H
#define DISASSEMBLERTYPES_H

#include <unordered_map>

namespace Disassembler {

// Instruction written out in little-endian
typedef struct RawInstruction {
    unsigned char byte1;
    unsigned char byte2;
    unsigned char byte3;
    unsigned char byte4;
} RawInstruction;

typedef struct Instruction {
} Instruction;

enum class Opcode : int {
    LOAD = 3,
    IMM_INSTR = 19,
    AUIPC = 23,
    S_TYPE = 35,
    R_TYPE = 51,
    LUI = 55,
    B_TYPE = 99,
    JALR = 103,
    JAL = 111,
    ENV_TYPE = 115,
};

static const std::unordered_map<unsigned char, Opcode> opcodeMap = {
    {0x03, Opcode::LOAD},    {0x13, Opcode::IMM_INSTR}, {0x17, Opcode::AUIPC},
    {0x23, Opcode::S_TYPE},  {0x33, Opcode::R_TYPE},    {0x37, Opcode::LUI},
    {0x63, Opcode::B_TYPE},  {0x67, Opcode::JALR},      {0x6F, Opcode::JAL},
    {0x73, Opcode::ENV_TYPE}};

}  // namespace Disassembler

#endif