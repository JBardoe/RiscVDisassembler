#ifndef RISCVTYPES_H
#define RISCVTYPES_H

#include <string>
#include <string_view>
#include <unordered_map>

#include "parser/ELFFile.hpp"

namespace Disassembler {

/**
 * All possible RISC-V opcodes
 */
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

/**
 * Constant lookup table to match opcodes to the enum
 */
static const std::unordered_map<unsigned char, Opcode> opcodeMap = {
    {0x03, Opcode::LOAD},    {0x13, Opcode::IMM_INSTR}, {0x17, Opcode::AUIPC},
    {0x23, Opcode::S_TYPE},  {0x33, Opcode::R_TYPE},    {0x37, Opcode::LUI},
    {0x63, Opcode::B_TYPE},  {0x67, Opcode::JALR},      {0x6F, Opcode::JAL},
    {0x73, Opcode::ENV_TYPE}};

/**
 * All RISC-V registers (underlying int value equals the address)
 */
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

/**
 * toString for the register enum
 *
 * @param r register enum
 * @return string equivalent
 */
std::string to_string(Register r);

/**
 * All RISC-V instructions
 */
enum class Operator : int {
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

/**
 * toString for the instruction enum
 *
 * @param i instruction enum
 * @return string equivalent
 */
std::string to_string(Operator i);

/**
 * Relevant types for entries in the symbol table
 * Underlying value is the equivalent constant value that represents the type in
 * the file
 */
enum class SymbolType : int {
    NOTYPE = 0,
    OBJECT = 1,
    FUNCTION = 2,
    SECTION = 3,
    FILE = 4,
    COMMON = 5,
    TLS = 6,
    LOOS = 10,
    HIOS = 12,
    LOPROC = 13,
    HIPROC = 15  // TODO bounds checking
};

/**
 * toString for the symbol type enum
 *
 * @param t symbol type enum
 * @return string equivalent
 */
std::string to_string(SymbolType t);

/**
 * All possible bindings for entries in the symbol table
 * Underlying value is the equivalent constant value that represents the binding
 * in the file
 */
enum class SymbolBinding : int {
    LOCAL = 0,
    GLOBAL = 1,
    WEAK = 2,
    LOOS = 10,
    HIOS = 12,
    LOPROC = 13,
    HIPROC = 15,
};

/**
 * toString for the symbol binding enum
 *
 * @param b symbol binding enum
 * @return string equivalent
 */
std::string to_string(SymbolBinding b);

/**
 * Represents an entry in the ELF symbol table
 */
typedef struct Symbol {
    std::string name;  // Name of the symbol
    uint32_t addr;  // Value of the symbol (relative/absolute address or offset
                    // into a section)
    uint32_t size;
    SymbolType type;          // Type of the symbol (e.g. function)
    SymbolBinding binding;    // Binding of the symbol (e.g. local)
    std::string sectionName;  // Name of the section to which the symbol is tied
} Symbol;

std::string to_string(Symbol s);

typedef struct Variable {
    std::string name;
    uint32_t addr;
    uint32_t val;
} Variable;

std::string to_string(Variable v);

}  // namespace Disassembler

#endif