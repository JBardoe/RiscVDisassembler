#include "disassembler/Instruction.hpp"

#include <iostream>

#include "disassembler/RiscvTypes.hpp"
#include "utils/DisassemblyException.hpp"

namespace Disassembler {

RInstruction::RInstruction(Disassembler::Opcode op, uint32_t raw) : op(op) {
    switch ((raw >> 12) & 0x07) {
        case 0:
            this->instr = (raw >> 25) ? Disassembler::Operator::sub
                                      : Disassembler::Operator::add;
            break;
        case 1:
            this->instr = Disassembler::Operator::sll;
            break;
        case 2:
            this->instr = Disassembler::Operator::slt;
            break;
        case 3:
            this->instr = Disassembler::Operator::sltu;
            break;
        case 4:
            this->instr = Disassembler::Operator::Xor;
            break;
        case 5:
            this->instr = (raw >> 25) ? Disassembler::Operator::sra
                                      : Disassembler::Operator::srl;
            break;
        case 6:
            this->instr = Disassembler::Operator::Or;
            break;
        case 7:
            this->instr = Disassembler::Operator::And;
            break;
        default:
            throw DisassemblyException(
                "Invalid funct3 parameter on R-Type instruction.");
    }

    // Registers to be in the range 0-31 as it is parsed from a 5 bit number
    this->rd = static_cast<Disassembler::Register>((raw >> 7) & 0x1F);
    this->rs1 = static_cast<Disassembler::Register>((raw >> 15) & 0x1F);
    this->rs2 = static_cast<Disassembler::Register>((raw >> 20) & 0x1F);
}

const std::string& RInstruction::toString() {
    if (this->printOut != "") return this->printOut;

    std::string symbol;

    this->printOut = "";

    this->printOut += Disassembler::to_string(this->instr) + " " +
                      Disassembler::to_string(this->rd) + ", " +
                      Disassembler::to_string(this->rs1) + ", " +
                      Disassembler::to_string(this->rs2) + "\t\t# " +
                      Disassembler::to_string(this->rd) + " = ";

    switch (this->instr) {
        case Disassembler::Operator::add:
            symbol = " + ";
            break;
        case Disassembler::Operator::sub:
            symbol = " - ";
            break;
        case Disassembler::Operator::Xor:
            symbol = " ^ ";
            break;
        case Disassembler::Operator::Or:
            symbol = " | ";
            break;
        case Disassembler::Operator::And:
            symbol = " & ";
            break;
        case Disassembler::Operator::sll:
            symbol = " << ";
            break;
        case Disassembler::Operator::srl:
            symbol = " >> ";
            break;
        case Disassembler::Operator::sra:
            symbol = " >> ";
        default:
            break;
    }

    if (this->instr == Disassembler::Operator::slt ||
        this->instr == Disassembler::Operator::sltu) {
        this->printOut += "(" + Disassembler::to_string(this->rs1) + " < " +
                          Disassembler::to_string(this->rs2) + ")?1:0";
    } else {
        this->printOut += Disassembler::to_string(this->rs1) + symbol +
                          Disassembler::to_string(this->rs2);
    }

    return this->printOut;
}

IInstruction::IInstruction(Disassembler::Opcode op, uint32_t raw) : op(op) {
    this->rd = static_cast<Disassembler::Register>((raw >> 7) & 0x1F);
    this->rs1 = static_cast<Disassembler::Register>((raw >> 15) & 0x1F);
    this->imm = (raw >> 20);

    this->imm = (this->imm << 20) >> 20;

    if (this->op == Disassembler::Opcode::JALR) {
        this->instr = Disassembler::Operator::jalr;
    } else if (this->op == Disassembler::Opcode::ENV_TYPE && this->imm == 0) {
        this->instr = Disassembler::Operator::ecall;
    } else if (this->op == Disassembler::Opcode::ENV_TYPE && this->imm == 1) {
        this->instr = Disassembler::Operator::ebreak;
    } else if (this->op == Disassembler::Opcode::LOAD) {
        switch ((raw >> 12) & 0x07) {
            case 0:
                this->instr = Disassembler::Operator::lb;
                break;
            case 1:
                this->instr = Disassembler::Operator::lh;
                break;
            case 2:
                this->instr = Disassembler::Operator::lw;
                break;
            case 4:
                this->instr = Disassembler::Operator::lbu;
                this->imm = (raw >> 20);
                break;
            case 5:
                this->instr = Disassembler::Operator::lhu;
                this->imm = (raw >> 20);
                break;
            default:
                throw DisassemblyException(
                    "Invalid funct3 parameter on I-Type instruction.");
        }
    } else {
        switch ((raw >> 12) & 0x07) {
            case 0:
                this->instr = Disassembler::Operator::addi;
                break;
            case 1:
                this->instr = Disassembler::Operator::slli;
                break;
            case 2:
                this->instr = Disassembler::Operator::slti;
                break;
            case 3:
                this->instr = Disassembler::Operator::sltiu;
                this->imm = (raw >> 20);
                break;
            case 4:
                this->instr = Disassembler::Operator::xori;
                break;
            case 5:
                if (((imm >> 5) & 0x7F) == 2) {
                    this->instr = Disassembler::Operator::srai;
                } else if (((imm >> 5) & 0x7F) == 0) {
                    this->instr = Disassembler::Operator::srli;
                } else {
                    throw DisassemblyException(
                        "Invalid imm parameter on Shift Right instruction.");
                }
                break;
            case 6:
                this->instr = Disassembler::Operator::ori;
                break;
            case 7:
                this->instr = Disassembler::Operator::andi;
                break;
            default:
                throw DisassemblyException(
                    "Invalid funct3 parameter on I-Type instruction.");
        }
    }
}

const std::string& IInstruction::toString() {
    if (this->printOut != "") return this->printOut;

    if (this->instr == Disassembler::Operator::jalr) {
        this->printOut = Disassembler::to_string(this->instr) +
                         Disassembler::to_string(this->rd) + ", " +
                         std::to_string(this->imm) + "(" +
                         Disassembler::to_string(rs1) + ")\t\t# " +
                         Disassembler::to_string(this->rd) +
                         " = PC+4; PC = " + Disassembler::to_string(this->rs1) +
                         " + " + std::to_string(this->imm);

        return this->printOut;
    }

    if (this->instr == Disassembler::Operator::ecall) {
        this->printOut = "ecall\t\t# Transfer control to OS";
        return this->printOut;
    }

    if (this->instr == Disassembler::Operator::ebreak) {
        this->printOut = "ebreak\t\t# Transfer control to debugger";
        return this->printOut;
    }

    if (this->op == Disassembler::Opcode::LOAD) {
        this->printOut = Disassembler::to_string(this->instr) + " " +
                         Disassembler::to_string(this->rd) + ", " +
                         std::to_string(this->imm) + "(" +
                         Disassembler::to_string(this->rs1) + ")\t\t# " +
                         Disassembler::to_string(this->rd) + " = M[" +
                         Disassembler::to_string(this->rs1) + "+" +
                         std::to_string(this->imm) + "][0:";

        switch (this->instr) {
            case Disassembler::Operator::lb:
            case Disassembler::Operator::lbu:
                this->printOut += std::to_string(7) + "]";
                break;
            case Disassembler::Operator::lh:
            case Disassembler::Operator::lhu:
                this->printOut += std::to_string(15) + "]";
                break;
            case Disassembler::Operator::lw:
                this->printOut += std::to_string(31) + "]";
                break;
            default:
                break;
        }

        return this->printOut;
    }

    this->printOut = Disassembler::to_string(this->instr) + " " +
                     Disassembler::to_string(this->rd) + ", " +
                     Disassembler::to_string(this->rs1) + ", " +
                     std::to_string(this->imm) + "\t\t# " +
                     Disassembler::to_string(this->rd) + " = ";

    std::string symbol;
    switch (this->instr) {
        case Disassembler::Operator::addi:
            symbol = " + ";
            break;
        case Disassembler::Operator::slli:
            symbol = " << ";
            break;
        case Disassembler::Operator::xori:
            symbol = " ^ ";
            break;
        case Disassembler::Operator::srai:
        case Disassembler::Operator::srli:
            symbol = " >> ";
            break;
        case Disassembler::Operator::ori:
            symbol = " | ";
            break;
        case Disassembler::Operator::andi:
            symbol = " & ";
            break;
        default:
            break;
    }

    if (this->instr == Disassembler::Operator::slti ||
        this->instr == Disassembler::Operator::sltiu) {
        this->printOut += "(" + Disassembler::to_string(this->rs1) + " < " +
                          std::to_string(this->imm) + ")?1:0";
    } else {
        this->printOut += Disassembler::to_string(this->rs1) + symbol +
                          std::to_string(this->imm);
    }

    if (this->instr == Disassembler::Operator::slli ||
        this->instr == Disassembler::Operator::srli ||
        this->instr == Disassembler::Operator::srai) {
        this->printOut += "[0:4]";
    }

    return this->printOut;
}

SInstruction::SInstruction(Disassembler::Opcode op, uint32_t raw) : op(op) {
    this->rs1 = static_cast<Disassembler::Register>((raw >> 15) & 0x1F);
    this->rs2 = static_cast<Disassembler::Register>((raw >> 20) & 0x1F);

    switch ((raw >> 12) & 0x07) {
        case 0:
            this->instr = Disassembler::Operator::sb;
            break;
        case 1:
            this->instr = Disassembler::Operator::sh;
            break;
        case 2:
            this->instr = Disassembler::Operator::sw;
            break;
        default:
            throw DisassemblyException(
                "Invalid funct3 parameter on S-Type instruction.");
    }

    this->imm = ((raw >> 7) & 0x17) | ((raw >> 25) << 5);
    this->imm = (this->imm << 20) >> 20;
}

const std::string& SInstruction::toString() {
    if (this->printOut != "") return this->printOut;
    int upper = 0;

    switch (this->instr) {
        case Disassembler::Operator::sb:
            upper = 7;
            break;
        case Disassembler::Operator::sh:
            upper = 15;
            break;
        case Disassembler::Operator::sw:
            upper = 31;
            break;
        default:
            break;
    }

    this->printOut =
        Disassembler::to_string(this->instr) + " " +
        Disassembler::to_string(this->rs2) + ", " + std::to_string(this->imm) +
        "(" + Disassembler::to_string(this->rs1) + ")" + "\t\t# M[" +
        Disassembler::to_string(this->rs1) + "+" + std::to_string(this->imm) +
        "][0:" + std::to_string(upper) +
        "] = " + Disassembler::to_string(this->rs2) +
        "[0:" + std::to_string(upper) + "]";

    return this->printOut;
}

BInstruction::BInstruction(Disassembler::Opcode op, uint32_t raw) : op(op) {
    switch ((raw >> 12) & 0x07) {
        case 0:
            this->instr = Disassembler::Operator::beq;
            break;
        case 1:
            this->instr = Disassembler::Operator::bne;
            break;
        case 4:
            this->instr = Disassembler::Operator::blt;
            break;
        case 5:
            this->instr = Disassembler::Operator::bge;
            break;
        case 6:
            this->instr = Disassembler::Operator::bltu;
            break;
        case 7:
            this->instr = Disassembler::Operator::bgeu;
            break;
        default:
            throw DisassemblyException(
                "Invalid funct3 parameter on B-Type instruction.");
    }

    this->rs1 = static_cast<Disassembler::Register>((raw >> 15) & 0x1F);
    this->rs2 = static_cast<Disassembler::Register>((raw >> 20) & 0x1F);

    this->imm = (((raw >> 31) & 0x01) << 12) | (((raw >> 7) & 0x01) << 11) |
                (((raw >> 25) & 0x3F) << 5) | (((raw >> 8) & 0x0F) << 1);

    if (this->instr != Disassembler::Operator::bltu &&
        this->instr != Disassembler::Operator::bgeu)
        this->imm = (this->imm << 19) >> 19;
}

const std::string& BInstruction::toString() {
    if (this->printOut != "") return this->printOut;

    std::string symbol;

    switch (this->instr) {
        case Disassembler::Operator::beq:
            symbol = "==";
            break;
        case Disassembler::Operator::bne:
            symbol = "!=";
            break;
        case Disassembler::Operator::blt:
            symbol = "<";
            break;
        case Disassembler::Operator::bge:
            symbol = ">=";
            break;
        case Disassembler::Operator::bltu:
            symbol = "<";
            break;
        case Disassembler::Operator::bgeu:
            symbol = ">=";
            break;
        default:
            break;
    }

    this->printOut = Disassembler::to_string(this->instr) + " " +
                     Disassembler::to_string(this->rs1) + ", " +
                     Disassembler::to_string(this->rs2) + ", " +
                     std::to_string(this->imm) + "\t\t# if(" +
                     Disassembler::to_string(this->rs1) + " " + symbol + " " +
                     Disassembler::to_string(this->rs2) +
                     ") PC += " + std::to_string(this->imm);

    return this->printOut;
}

UInstruction::UInstruction(Disassembler::Opcode op, uint32_t raw) : op(op) {
    this->instr = (op == Disassembler::Opcode::LUI)
                      ? Disassembler::Operator::lui
                      : Disassembler::Operator::auipc;
    this->rd = static_cast<Disassembler::Register>((raw >> 7) & 0x1F);
    this->imm = (raw >> 12);
}

const std::string& UInstruction::toString() {
    if (this->printOut != "") return this->printOut;

    this->printOut = Disassembler::to_string(this->instr) + " " +
                     Disassembler::to_string(this->rd) + ", " +
                     std::to_string(this->imm) + "\t\t# " +
                     Disassembler::to_string(this->rd) + " = ";

    // Cannot be an unknown opcode as it will have been caught earlier
    if (this->op == Disassembler::Opcode::LUI) {
        this->printOut += std::to_string(this->imm) + " << 12";
    } else {
        this->printOut += "PC + (" + std::to_string(this->imm) + " << 12)";
    }

    return this->printOut;
}

JInstruction::JInstruction(Disassembler::Opcode op, uint32_t raw) : op(op) {
    this->instr = Disassembler::Operator::jal;
    this->rd = static_cast<Disassembler::Register>((raw >> 7) & 0x1F);
    this->imm = (((raw >> 31) & 0x1) << 20) | (((raw >> 12) & 0xFF) << 12) |
                (((raw >> 20) & 0x1) << 11) | (((raw >> 21) & 0x3FF) << 1);

    this->imm = (this->imm << 11) >> 11;
}

const std::string& JInstruction::toString() {
    if (this->printOut != "") return this->printOut;

    this->printOut = Disassembler::to_string(this->instr) +
                     Disassembler::to_string(this->rd) + ", " +
                     std::to_string(this->imm) + "\t\t# " +
                     Disassembler::to_string(this->rd) +
                     " = PC+$; PC += " + std::to_string(this->imm);

    return this->printOut;
}

}  // namespace Disassembler