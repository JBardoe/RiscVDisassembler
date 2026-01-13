#ifndef DISASSEMBLERTYPES_H
#define DISASSEMBLERTYPES_H

#include <array>
#include <string_view>
#include <unordered_map>

namespace Disassembler {

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

constexpr std::array<std::string_view, 32> registerNames = {
    "zero", "ra", "sp", "gp", "tp",  "t0",  "t1", "t2", "s0/fp", "s1", "a0",
    "a1",   "a2", "a3", "a4", "a5",  "a6",  "a7", "s2", "s3",    "s4", "s5",
    "s6",   "s7", "s8", "s9", "s10", "s11", "t3", "t4", "t5",    "t6"};

}  // namespace Disassembler

#endif