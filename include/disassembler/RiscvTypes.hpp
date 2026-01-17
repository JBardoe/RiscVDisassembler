#ifndef RISCVTYPES_H
#define RISCVTYPES_H

#include <array>
#include <string>
#include <string_view>
#include <unordered_map>

#include "disassembler/ELFFile.hpp"

namespace RISCV {

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

enum class Register : int {
    zero,
    ra,
    sp,
    gp,
    tp,
    t0,
    t1,
    t2,
    s0fp,
    s1,
    a0,
    a1,
    a2,
    a3,
    a4,
    a5,
    a6,
    a7,
    s2,
    s3,
    s4,
    s5,
    s6,
    s7,
    s8,
    s9,
    s10,
    s11,
    t3,
    t4,
    t5,
    t6,
};

std::string to_string(Register r);

enum class Instruction : int {
    add,
    sub,
    Xor,
    Or,
    And,
    sll,
    srl,
    sra,
    slt,
    sltu,
    addi,
    xori,
    ori,
    andi,
    slli,
    srli,
    srai,
    slti,
    sltiu,
    lb,
    lh,
    lw,
    lbu,
    lhu,
    sb,
    sh,
    sw,
    beq,
    bne,
    blt,
    bge,
    bltu,
    bgeu,
    jal,
    jalr,
    lui,
    auipc,
    ecall,
    ebreak,
};

std::string to_string(Instruction i);

enum class SymbolType : int {
    OBJECT = 1,
    FUNCTION = 2,
};

std::string to_string(SymbolType t);

enum class SymbolBinding : int {
    LOCAL = 0,
    GLOBAL = 1,
    WEAK = 2,
    LOOS = 10,
    HIOS = 12,
    LOPROC = 13,
    HIPROC = 15,
};

std::string to_string(SymbolBinding b);

typedef struct Symbol {
    std::string name;
    uint32_t addr;
    uint32_t size;
    SymbolType type;
    SymbolBinding binding;
    std::string sectionName;
} Symbol;

}  // namespace RISCV

#endif