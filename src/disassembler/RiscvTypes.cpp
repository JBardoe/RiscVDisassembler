#include "disassembler/RiscvTypes.hpp"

namespace RISCV {
std::string to_string(Register r) {
    switch (r) {
        case Register::zero:
            return "zero";
        case Register::ra:
            return "ra";
        case Register::sp:
            return "sp";
        case Register::gp:
            return "gp";
        case Register::tp:
            return "tp";
        case Register::t0:
            return "t0";
        case Register::t1:
            return "t1";
        case Register::t2:
            return "t2";
        case Register::s0fp:
            return "s0/fp";
        case Register::s1:
            return "s1";
        case Register::a0:
            return "a0";
        case Register::a1:
            return "a1";
        case Register::a2:
            return "a2";
        case Register::a3:
            return "a3";
        case Register::a4:
            return "a4";
        case Register::a5:
            return "a5";
        case Register::a6:
            return "a6";
        case Register::a7:
            return "a7";
        case Register::s2:
            return "s2";
        case Register::s3:
            return "s3";
        case Register::s4:
            return "s4";
        case Register::s5:
            return "s5";
        case Register::s6:
            return "s6";
        case Register::s7:
            return "s7";
        case Register::s8:
            return "s8";
        case Register::s9:
            return "s9";
        case Register::s10:
            return "s10";
        case Register::s11:
            return "s11";
        case Register::t3:
            return "t3";
        case Register::t4:
            return "t4";
        case Register::t5:
            return "t5";
        case Register::t6:
            return "t6";
        default:
            return "<unknown_register>";
    }
}

std::string to_string(Instruction i) {
    switch (i) {
        case Instruction::add:
            return "add";
        case Instruction::sub:
            return "sub";
        case Instruction::Xor:
            return "xor";
        case Instruction::Or:
            return "or";
        case Instruction::And:
            return "and";
        case Instruction::sll:
            return "sll";
        case Instruction::srl:
            return "srl";
        case Instruction::sra:
            return "sra";
        case Instruction::slt:
            return "slt";
        case Instruction::sltu:
            return "sltu";
        case Instruction::addi:
            return "addi";
        case Instruction::xori:
            return "xori";
        case Instruction::ori:
            return "ori";
        case Instruction::andi:
            return "andi";
        case Instruction::srli:
            return "srli";
        case Instruction::srai:
            return "srai";
        case Instruction::slti:
            return "slti";
        case Instruction::sltiu:
            return "sltiu";
        case Instruction::lb:
            return "lb";
        case Instruction::lh:
            return "lh";
        case Instruction::lw:
            return "lw";
        case Instruction::lbu:
            return "lbu";
        case Instruction::lhu:
            return "lhu";
        case Instruction::sb:
            return "sb";
        case Instruction::sh:
            return "sh";
        case Instruction::sw:
            return "sw";
        case Instruction::beq:
            return "beq";
        case Instruction::bne:
            return "bne";
        case Instruction::blt:
            return "blt";
        case Instruction::bge:
            return "bge";
        case Instruction::bltu:
            return "bltu";
        case Instruction::bgeu:
            return "bgeu";
        case Instruction::jal:
            return "jal";
        case Instruction::jalr:
            return "jalr";
        case Instruction::lui:
            return "lui";
        case Instruction::auipc:
            return "auipc";
        case Instruction::ecall:
            return "ecall";
        case Instruction::ebreak:
            return "ebreak";
        default:
            return "<unknown_instruction>";
    }
}

std::string to_string(SymbolType t) {
    switch (t) {
        case SymbolType::OBJECT:
            return "object";
        case SymbolType::FUNCTION:
            return "function";
    }
    return "<unknown_type>";
}

std::string to_string(SymbolBinding b) {
    switch (b) {
        case SymbolBinding::LOCAL:
            return "local";
        case SymbolBinding::GLOBAL:
            return "global";
        case SymbolBinding::WEAK:
            return "weak";
        case SymbolBinding::LOOS:
            return "loos";
        case SymbolBinding::HIOS:
            return "hios";
        case SymbolBinding::LOPROC:
            return "loproc";
        case SymbolBinding::HIPROC:
            return "hiproc";
    }
    return "<unknown_binding>";
}

}  // namespace RISCV