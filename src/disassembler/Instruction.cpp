#include "disassembler/Instruction.hpp"

#include <iostream>

#include "disassembler/RiscvTypes.hpp"
#include "utils/DisassemblyException.hpp"

namespace Disassembler {

RInstruction::RInstruction(RISCV::Opcode op, uint32_t raw) : op(op) {
    switch ((raw >> 12) & 0x07) {
        case 0:
            this->instr =
                (raw >> 25) ? RISCV::Instruction::sub : RISCV::Instruction::add;
            break;
        case 1:
            this->instr = RISCV::Instruction::sll;
            break;
        case 2:
            this->instr = RISCV::Instruction::slt;
            break;
        case 3:
            this->instr = RISCV::Instruction::sltu;
            break;
        case 4:
            this->instr = RISCV::Instruction::Xor;
            break;
        case 5:
            this->instr =
                (raw >> 25) ? RISCV::Instruction::sra : RISCV::Instruction::srl;
            break;
        case 6:
            this->instr = RISCV::Instruction::Or;
            break;
        case 7:
            this->instr = RISCV::Instruction::And;
            break;
        default:
            throw DisassemblyException(
                "Invalid funct3 parameter on R-Type instruction.");
    }

    // Registers to be in the range 0-31 as it is parsed from a 5 bit number
    this->rd = static_cast<RISCV::Register>((raw >> 7) & 0x1F);
    this->rs1 = static_cast<RISCV::Register>((raw >> 15) & 0x1F);
    this->rs2 = static_cast<RISCV::Register>((raw >> 20) & 0x1F);
}

const std::string& RInstruction::toString() {
    if (this->printOut != "") return this->printOut;

    std::string symbol;

    this->printOut = "";

    this->printOut += RISCV::to_string(this->instr) + " " +
                      RISCV::to_string(this->rd) + ", " +
                      RISCV::to_string(this->rs1) + ", " +
                      RISCV::to_string(this->rs2) + "\t\t# " +
                      RISCV::to_string(this->rd) + " = ";

    switch (this->instr) {
        case RISCV::Instruction::add:
            symbol = " + ";
            break;
        case RISCV::Instruction::sub:
            symbol = " - ";
            break;
        case RISCV::Instruction::Xor:
            symbol = " ^ ";
            break;
        case RISCV::Instruction::Or:
            symbol = " | ";
            break;
        case RISCV::Instruction::And:
            symbol = " & ";
            break;
        case RISCV::Instruction::sll:
            symbol = " << ";
            break;
        case RISCV::Instruction::srl:
            symbol = " >> ";
            break;
        case RISCV::Instruction::sra:
            symbol = " >> ";
        default:
            break;
    }

    if (this->instr == RISCV::Instruction::slt ||
        this->instr == RISCV::Instruction::sltu) {
        this->printOut += "(" + RISCV::to_string(this->rs1) + " < " +
                          RISCV::to_string(this->rs2) + ")?1:0";
    } else {
        this->printOut +=
            RISCV::to_string(this->rs1) + symbol + RISCV::to_string(this->rs2);
    }

    return this->printOut;
}

IInstruction::IInstruction(RISCV::Opcode op, uint32_t raw) : op(op) {
    this->rd = static_cast<RISCV::Register>((raw >> 7) & 0x1F);
    this->rs1 = static_cast<RISCV::Register>((raw >> 15) & 0x1F);
    this->imm = (raw >> 20);

    this->imm = (this->imm << 20) >> 20;

    if (this->op == RISCV::Opcode::JALR) {
        this->instr = RISCV::Instruction::jalr;
    } else if (this->op == RISCV::Opcode::ENV_TYPE && this->imm == 0) {
        this->instr = RISCV::Instruction::ecall;
    } else if (this->op == RISCV::Opcode::ENV_TYPE && this->imm == 1) {
        this->instr = RISCV::Instruction::ebreak;
    } else if (this->op == RISCV::Opcode::LOAD) {
        switch ((raw >> 12) & 0x07) {
            case 0:
                this->instr = RISCV::Instruction::lb;
                break;
            case 1:
                this->instr = RISCV::Instruction::lh;
                break;
            case 2:
                this->instr = RISCV::Instruction::lw;
                break;
            case 4:
                this->instr = RISCV::Instruction::lbu;
                this->imm = (raw >> 20);
                break;
            case 5:
                this->instr = RISCV::Instruction::lhu;
                this->imm = (raw >> 20);
                break;
            default:
                throw DisassemblyException(
                    "Invalid funct3 parameter on I-Type instruction.");
        }
    } else {
        switch ((raw >> 12) & 0x07) {
            case 0:
                this->instr = RISCV::Instruction::addi;
                break;
            case 1:
                this->instr = RISCV::Instruction::slli;
                break;
            case 2:
                this->instr = RISCV::Instruction::slti;
                break;
            case 3:
                this->instr = RISCV::Instruction::sltiu;
                this->imm = (raw >> 20);
                break;
            case 4:
                this->instr = RISCV::Instruction::xori;
                break;
            case 5:
                if (((imm >> 5) & 0x7F) == 2) {
                    this->instr = RISCV::Instruction::srai;
                } else if (((imm >> 5) & 0x7F) == 0) {
                    this->instr = RISCV::Instruction::srli;
                } else {
                    throw DisassemblyException(
                        "Invalid imm parameter on Shift Right instruction.");
                }
                break;
            case 6:
                this->instr = RISCV::Instruction::ori;
                break;
            case 7:
                this->instr = RISCV::Instruction::andi;
                break;
            default:
                throw DisassemblyException(
                    "Invalid funct3 parameter on I-Type instruction.");
        }
    }
}

const std::string& IInstruction::toString() {
    if (this->printOut != "") return this->printOut;

    if (this->instr == RISCV::Instruction::jalr) {
        this->printOut =
            RISCV::to_string(this->instr) + RISCV::to_string(this->rd) + ", " +
            std::to_string(this->imm) + "(" + RISCV::to_string(rs1) +
            ")\t\t# " + RISCV::to_string(this->rd) +
            " = PC+4; PC = " + RISCV::to_string(this->rs1) + " + " +
            std::to_string(this->imm);

        return this->printOut;
    }

    if (this->instr == RISCV::Instruction::ecall) {
        this->printOut = "ecall\t\t# Transfer control to OS";
        return this->printOut;
    }

    if (this->instr == RISCV::Instruction::ebreak) {
        this->printOut = "ebreak\t\t# Transfer control to debugger";
        return this->printOut;
    }

    if (this->op == RISCV::Opcode::LOAD) {
        this->printOut =
            RISCV::to_string(this->instr) + " " + RISCV::to_string(this->rd) +
            ", " + std::to_string(this->imm) + "(" +
            RISCV::to_string(this->rs1) + ")\t\t# " +
            RISCV::to_string(this->rd) + " = M[" + RISCV::to_string(this->rs1) +
            "+" + std::to_string(this->imm) + "][0:";

        switch (this->instr) {
            case RISCV::Instruction::lb:
            case RISCV::Instruction::lbu:
                this->printOut += std::to_string(7) + "]";
                break;
            case RISCV::Instruction::lh:
            case RISCV::Instruction::lhu:
                this->printOut += std::to_string(15) + "]";
                break;
            case RISCV::Instruction::lw:
                this->printOut += std::to_string(31) + "]";
                break;
            default:
                break;
        }

        return this->printOut;
    }

    this->printOut =
        RISCV::to_string(this->instr) + " " + RISCV::to_string(this->rd) +
        ", " + RISCV::to_string(this->rs1) + ", " + std::to_string(this->imm) +
        "\t\t# " + RISCV::to_string(this->rd) + " = ";

    std::string symbol;
    switch (this->instr) {
        case RISCV::Instruction::addi:
            symbol = " + ";
            break;
        case RISCV::Instruction::slli:
            symbol = " << ";
            break;
        case RISCV::Instruction::xori:
            symbol = " ^ ";
            break;
        case RISCV::Instruction::srai:
        case RISCV::Instruction::srli:
            symbol = " >> ";
            break;
        case RISCV::Instruction::ori:
            symbol = " | ";
            break;
        case RISCV::Instruction::andi:
            symbol = " & ";
            break;
        default:
            break;
    }

    if (this->instr == RISCV::Instruction::slti ||
        this->instr == RISCV::Instruction::sltiu) {
        this->printOut += "(" + RISCV::to_string(this->rs1) + " < " +
                          std::to_string(this->imm) + ")?1:0";
    } else {
        this->printOut +=
            RISCV::to_string(this->rs1) + symbol + std::to_string(this->imm);
    }

    if (this->instr == RISCV::Instruction::slli ||
        this->instr == RISCV::Instruction::srli ||
        this->instr == RISCV::Instruction::srai) {
        this->printOut += "[0:4]";
    }

    return this->printOut;
}

SInstruction::SInstruction(RISCV::Opcode op, uint32_t raw) : op(op) {
    this->rs1 = static_cast<RISCV::Register>((raw >> 15) & 0x1F);
    this->rs2 = static_cast<RISCV::Register>((raw >> 20) & 0x1F);

    switch ((raw >> 12) & 0x07) {
        case 0:
            this->instr = RISCV::Instruction::sb;
            break;
        case 1:
            this->instr = RISCV::Instruction::sh;
            break;
        case 2:
            this->instr = RISCV::Instruction::sw;
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
        case RISCV::Instruction::sb:
            upper = 7;
            break;
        case RISCV::Instruction::sh:
            upper = 15;
            break;
        case RISCV::Instruction::sw:
            upper = 31;
            break;
        default:
            break;
    }

    this->printOut =
        RISCV::to_string(this->instr) + " " + RISCV::to_string(this->rs2) +
        ", " + std::to_string(this->imm) + "(" + RISCV::to_string(this->rs1) +
        ")" + "\t\t# M[" + RISCV::to_string(this->rs1) + "+" +
        std::to_string(this->imm) + "][0:" + std::to_string(upper) +
        "] = " + RISCV::to_string(this->rs2) + "[0:" + std::to_string(upper) +
        "]";

    return this->printOut;
}

BInstruction::BInstruction(RISCV::Opcode op, uint32_t raw) : op(op) {
    switch ((raw >> 12) & 0x07) {
        case 0:
            this->instr = RISCV::Instruction::beq;
            break;
        case 1:
            this->instr = RISCV::Instruction::bne;
            break;
        case 4:
            this->instr = RISCV::Instruction::blt;
            break;
        case 5:
            this->instr = RISCV::Instruction::bge;
            break;
        case 6:
            this->instr = RISCV::Instruction::bltu;
            break;
        case 7:
            this->instr = RISCV::Instruction::bgeu;
            break;
        default:
            throw DisassemblyException(
                "Invalid funct3 parameter on B-Type instruction.");
    }

    this->rs1 = static_cast<RISCV::Register>((raw >> 15) & 0x1F);
    this->rs2 = static_cast<RISCV::Register>((raw >> 20) & 0x1F);

    this->imm = (((raw >> 31) & 0x01) << 12) | (((raw >> 7) & 0x01) << 11) |
                (((raw >> 25) & 0x3F) << 5) | (((raw >> 8) & 0x0F) << 1);

    if (this->instr != RISCV::Instruction::bltu &&
        this->instr != RISCV::Instruction::bgeu)
        this->imm = (this->imm << 19) >> 19;
}

const std::string& BInstruction::toString() {
    if (this->printOut != "") return this->printOut;

    std::string symbol;

    switch (this->instr) {
        case RISCV::Instruction::beq:
            symbol = "==";
            break;
        case RISCV::Instruction::bne:
            symbol = "!=";
            break;
        case RISCV::Instruction::blt:
            symbol = "<";
            break;
        case RISCV::Instruction::bge:
            symbol = ">=";
            break;
        case RISCV::Instruction::bltu:
            symbol = "<";
            break;
        case RISCV::Instruction::bgeu:
            symbol = ">=";
            break;
        default:
            break;
    }

    this->printOut =
        RISCV::to_string(this->instr) + " " + RISCV::to_string(this->rs1) +
        ", " + RISCV::to_string(this->rs2) + ", " + std::to_string(this->imm) +
        "\t\t# if(" + RISCV::to_string(this->rs1) + " " + symbol + " " +
        RISCV::to_string(this->rs2) + ") PC += " + std::to_string(this->imm);

    return this->printOut;
}

UInstruction::UInstruction(RISCV::Opcode op, uint32_t raw) : op(op) {
    this->instr = (op == RISCV::Opcode::LUI) ? RISCV::Instruction::lui
                                             : RISCV::Instruction::auipc;
    this->rd = static_cast<RISCV::Register>((raw >> 7) & 0x1F);
    this->imm = (raw >> 12);
}

const std::string& UInstruction::toString() {
    if (this->printOut != "") return this->printOut;

    this->printOut = RISCV::to_string(this->instr) + " " +
                     RISCV::to_string(this->rd) + ", " +
                     std::to_string(this->imm) + "\t\t# " +
                     RISCV::to_string(this->rd) + " = ";

    // Cannot be an unknown opcode as it will have been caught earlier
    if (this->op == RISCV::Opcode::LUI) {
        this->printOut += std::to_string(this->imm) + " << 12";
    } else {
        this->printOut += "PC + (" + std::to_string(this->imm) + " << 12)";
    }

    return this->printOut;
}

JInstruction::JInstruction(RISCV::Opcode op, uint32_t raw) : op(op) {
    this->instr = RISCV::Instruction::jal;
    this->rd = static_cast<RISCV::Register>((raw >> 7) & 0x1F);
    this->imm = (((raw >> 31) & 0x1) << 20) | (((raw >> 12) & 0xFF) << 12) |
                (((raw >> 20) & 0x1) << 11) | (((raw >> 21) & 0x3FF) << 1);

    this->imm = (this->imm << 11) >> 11;
}

const std::string& JInstruction::toString() {
    if (this->printOut != "") return this->printOut;

    this->printOut =
        RISCV::to_string(this->instr) + RISCV::to_string(this->rd) + ", " +
        std::to_string(this->imm) + "\t\t# " + RISCV::to_string(this->rd) +
        " = PC+$; PC += " + std::to_string(this->imm);

    return this->printOut;
}

}  // namespace Disassembler