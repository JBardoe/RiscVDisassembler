#ifndef RISCVTYPES_H
#define RISCVTYPES_H

#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>

#include "parser/ELFFile.hpp"
#include "utils/AssemblyTypes.hpp"

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

static const std::unordered_set<Register> tempRegisters{
    Register::t0, Register::t1, Register::t2, Register::t3,
    Register::t4, Register::t5, Register::t6};

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
    la,
    entry,  // Fake instruction denoting an entry point
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
    HIPROC = 15
};

/**
 * toString for the symbol type enum
 *
 * @param t symbol type enum
 * @return string equivalent
 */
std::string to_string(SymbolType t);

/**
 * Represents an entry in the ELF symbol table
 */
typedef struct Symbol {
    std::string name;  // Name of the symbol
    uint32_t addr;  // Value of the symbol (relative/absolute address or offset
                    // into a section)
    uint32_t size;
    SymbolType type;                  // Type of the symbol (e.g. function)
    Assembly::SymbolBinding binding;  // Binding of the symbol (e.g. local)
    std::string sectionName;  // Name of the section to which the symbol is tied
} Symbol;

/**
 * toString for the symbol enum
 *
 * @param s symbol enum
 * @return string equivalent
 */
std::string to_string(Symbol s);

/**
 * Represents possible type of the file (e.g. executable)
 */
enum class FileType : int {
    REL = 1,
    EXEC = 2,
    DYN = 3,
};

}  // namespace Disassembler

#endif