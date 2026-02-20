#include "disassembler/RiscvTypes.hpp"

namespace Disassembler {
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

std::string to_string(Operator i) {
    switch (i) {
        case Operator::add:
            return "add";
        case Operator::sub:
            return "sub";
        case Operator::Xor:
            return "xor";
        case Operator::Or:
            return "or";
        case Operator::And:
            return "and";
        case Operator::sll:
            return "sll";
        case Operator::srl:
            return "srl";
        case Operator::sra:
            return "sra";
        case Operator::slt:
            return "slt";
        case Operator::sltu:
            return "sltu";
        case Operator::addi:
            return "addi";
        case Operator::xori:
            return "xori";
        case Operator::ori:
            return "ori";
        case Operator::andi:
            return "andi";
        case Operator::srli:
            return "srli";
        case Operator::srai:
            return "srai";
        case Operator::slti:
            return "slti";
        case Operator::sltiu:
            return "sltiu";
        case Operator::lb:
            return "lb";
        case Operator::lh:
            return "lh";
        case Operator::lw:
            return "lw";
        case Operator::lbu:
            return "lbu";
        case Operator::lhu:
            return "lhu";
        case Operator::sb:
            return "sb";
        case Operator::sh:
            return "sh";
        case Operator::sw:
            return "sw";
        case Operator::beq:
            return "beq";
        case Operator::bne:
            return "bne";
        case Operator::blt:
            return "blt";
        case Operator::bge:
            return "bge";
        case Operator::bltu:
            return "bltu";
        case Operator::bgeu:
            return "bgeu";
        case Operator::jal:
            return "jal";
        case Operator::jalr:
            return "jalr";
        case Operator::lui:
            return "lui";
        case Operator::auipc:
            return "auipc";
        case Operator::ecall:
            return "ecall";
        case Operator::ebreak:
            return "ebreak";
        default:
            return "<unknown_instruction>";
    }
}

std::string to_string(SymbolType t) {
    switch (t) {
        case SymbolType::NOTYPE:
            return "no-type";
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

std::string to_string(Symbol s) {
    return "\tName: " + s.name + " | Address: " + std::to_string(s.addr) +
           " | Type: " + to_string(s.type) +
           " | Binding: " + to_string(s.binding) +
           " | Section: " + s.sectionName + "\n";
}

}  // namespace Disassembler