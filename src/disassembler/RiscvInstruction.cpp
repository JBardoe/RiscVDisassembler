#include <iostream>

#include "disassembler/RiscvInstruction.hpp"
#include "disassembler/RiscvTypes.hpp"
#include "utils/DisassemblyException.hpp"

namespace Disassembler {

RInstruction::RInstruction(Opcode op, uint32_t raw) : RiscvInstruction(op) {
    switch ((raw >> 12) & 0x07) {
        case 0:
            this->instr = (raw >> 25) ? Operator::sub : Operator::add;
            break;
        case 1:
            this->instr = Operator::sll;
            break;
        case 2:
            this->instr = Operator::slt;
            break;
        case 3:
            this->instr = Operator::sltu;
            break;
        case 4:
            this->instr = Operator::Xor;
            break;
        case 5:
            this->instr = (raw >> 25) ? Operator::sra : Operator::srl;
            break;
        case 6:
            this->instr = Operator::Or;
            break;
        case 7:
            this->instr = Operator::And;
            break;
        default:
            throw DisassemblyException(
                "Invalid funct3 parameter on R-Type instruction.");
    }

    // Registers to be in the range 0-31 as it is parsed from a 5 bit number
    this->rd = static_cast<Register>((raw >> 7) & 0x1F);
    this->rs1 = static_cast<Register>((raw >> 15) & 0x1F);
    this->rs2 = static_cast<Register>((raw >> 20) & 0x1F);
}

const std::string& RInstruction::toString() {
    if (this->printOut != "") return this->printOut;

    std::string symbol;

    this->printOut = "\t" + to_string(this->instr) + " " + to_string(this->rd) +
                     ", " + to_string(this->rs1) + ", " + to_string(this->rs2) +
                     "\t\t# " + to_string(this->rd) + " = ";

    switch (this->instr) {
        case Operator::add:
            symbol = " + ";
            break;
        case Operator::sub:
            symbol = " - ";
            break;
        case Operator::Xor:
            symbol = " ^ ";
            break;
        case Operator::Or:
            symbol = " | ";
            break;
        case Operator::And:
            symbol = " & ";
            break;
        case Operator::sll:
            symbol = " << ";
            break;
        case Operator::srl:
            symbol = " >> ";
            break;
        case Operator::sra:
            symbol = " >> ";
        default:
            break;
    }

    if (this->instr == Operator::slt || this->instr == Operator::sltu) {
        this->printOut +=
            "(" + to_string(this->rs1) + " < " + to_string(this->rs2) + ")?1:0";
    } else {
        this->printOut += to_string(this->rs1) + symbol + to_string(this->rs2);
    }

    return this->printOut;
}

IInstruction::IInstruction(Opcode op, uint32_t raw) : RiscvInstruction(op) {
    this->rd = static_cast<Register>((raw >> 7) & 0x1F);
    this->rs1 = static_cast<Register>((raw >> 15) & 0x1F);
    this->imm = (raw >> 20);

    this->imm = (this->imm << 20) >> 20;

    if (this->op == Opcode::JALR) {
        this->instr = Operator::jalr;
    } else if (this->op == Opcode::ENV_TYPE && this->imm == 0) {
        this->instr = Operator::ecall;
    } else if (this->op == Opcode::ENV_TYPE && this->imm == 1) {
        this->instr = Operator::ebreak;
    } else if (this->op == Opcode::LOAD) {
        switch ((raw >> 12) & 0x07) {
            case 0:
                this->instr = Operator::lb;
                break;
            case 1:
                this->instr = Operator::lh;
                break;
            case 2:
                this->instr = Operator::lw;
                break;
            case 4:
                this->instr = Operator::lbu;
                this->imm = (raw >> 20);
                break;
            case 5:
                this->instr = Operator::lhu;
                this->imm = (raw >> 20);
                break;
            default:
                throw DisassemblyException(
                    "Invalid funct3 parameter on I-Type instruction.");
        }
    } else {
        switch ((raw >> 12) & 0x07) {
            case 0:
                this->instr = Operator::addi;
                break;
            case 1:
                this->instr = Operator::slli;
                break;
            case 2:
                this->instr = Operator::slti;
                break;
            case 3:
                this->instr = Operator::sltiu;
                this->imm = (raw >> 20);
                break;
            case 4:
                this->instr = Operator::xori;
                break;
            case 5:
                if (((imm >> 5) & 0x7F) == 2) {
                    this->instr = Operator::srai;
                } else if (((imm >> 5) & 0x7F) == 0) {
                    this->instr = Operator::srli;
                } else {
                    throw DisassemblyException(
                        "Invalid imm parameter on Shift Right instruction.");
                }
                break;
            case 6:
                this->instr = Operator::ori;
                break;
            case 7:
                this->instr = Operator::andi;
                break;
            default:
                throw DisassemblyException(
                    "Invalid funct3 parameter on I-Type instruction.");
        }
    }
}

const std::string& IInstruction::toString() {
    if (this->printOut != "") return this->printOut;

    if (this->instr == Operator::jalr) {
        this->printOut = "\t" + to_string(this->instr) + to_string(this->rd) +
                         ", " + std::to_string(this->imm) + "(" +
                         to_string(rs1) + ")\t\t# " + to_string(this->rd) +
                         " = PC+4; PC = " + to_string(this->rs1) + " + " +
                         std::to_string(this->imm);

        return this->printOut;
    }

    if (this->instr == Operator::ecall) {
        this->printOut = "\tecall\t\t# Transfer control to OS";
        return this->printOut;
    }

    if (this->instr == Operator::ebreak) {
        this->printOut = "\tebreak\t\t# Transfer control to debugger";
        return this->printOut;
    }

    if (this->op == Opcode::LOAD) {
        this->printOut =
            "\t" + to_string(this->instr) + " " + to_string(this->rd) + ", " +
            std::to_string(this->imm) + "(" + to_string(this->rs1) + ")\t\t# " +
            to_string(this->rd) + " = M[" + to_string(this->rs1) + "+" +
            std::to_string(this->imm) + "][0:";

        switch (this->instr) {
            case Operator::lb:
            case Operator::lbu:
                this->printOut += std::to_string(7) + "]";
                break;
            case Operator::lh:
            case Operator::lhu:
                this->printOut += std::to_string(15) + "]";
                break;
            case Operator::lw:
                this->printOut += std::to_string(31) + "]";
                break;
            default:
                break;
        }

        return this->printOut;
    }

    this->printOut = "\t" + to_string(this->instr) + " " + to_string(this->rd) +
                     ", " + to_string(this->rs1) + ", " +
                     std::to_string(this->imm) + "\t\t# " +
                     to_string(this->rd) + " = ";

    std::string symbol;
    switch (this->instr) {
        case Operator::addi:
            symbol = " + ";
            break;
        case Operator::slli:
            symbol = " << ";
            break;
        case Operator::xori:
            symbol = " ^ ";
            break;
        case Operator::srai:
        case Operator::srli:
            symbol = " >> ";
            break;
        case Operator::ori:
            symbol = " | ";
            break;
        case Operator::andi:
            symbol = " & ";
            break;
        default:
            break;
    }

    if (this->instr == Operator::slti || this->instr == Operator::sltiu) {
        this->printOut += "(" + to_string(this->rs1) + " < " +
                          std::to_string(this->imm) + ")?1:0";
    } else {
        this->printOut +=
            to_string(this->rs1) + symbol + std::to_string(this->imm);
    }

    if (this->instr == Operator::slli || this->instr == Operator::srli ||
        this->instr == Operator::srai) {
        this->printOut += "[0:4]";
    }

    return this->printOut;
}

SInstruction::SInstruction(Opcode op, uint32_t raw) : RiscvInstruction(op) {
    this->rs1 = static_cast<Register>((raw >> 15) & 0x1F);
    this->rs2 = static_cast<Register>((raw >> 20) & 0x1F);

    switch ((raw >> 12) & 0x07) {
        case 0:
            this->instr = Operator::sb;
            break;
        case 1:
            this->instr = Operator::sh;
            break;
        case 2:
            this->instr = Operator::sw;
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
        case Operator::sb:
            upper = 7;
            break;
        case Operator::sh:
            upper = 15;
            break;
        case Operator::sw:
            upper = 31;
            break;
        default:
            break;
    }

    this->printOut =
        "\t" + to_string(this->instr) + " " + to_string(this->rs2) + ", " +
        std::to_string(this->imm) + "(" + to_string(this->rs1) + ")" +
        "\t\t# M[" + to_string(this->rs1) + "+" + std::to_string(this->imm) +
        "][0:" + std::to_string(upper) + "] = " + to_string(this->rs2) +
        "[0:" + std::to_string(upper) + "]";

    return this->printOut;
}

BInstruction::BInstruction(Opcode op, uint32_t raw) : RiscvInstruction(op) {
    switch ((raw >> 12) & 0x07) {
        case 0:
            this->instr = Operator::beq;
            break;
        case 1:
            this->instr = Operator::bne;
            break;
        case 4:
            this->instr = Operator::blt;
            break;
        case 5:
            this->instr = Operator::bge;
            break;
        case 6:
            this->instr = Operator::bltu;
            break;
        case 7:
            this->instr = Operator::bgeu;
            break;
        default:
            throw DisassemblyException(
                "Invalid funct3 parameter on B-Type instruction.");
    }

    this->rs1 = static_cast<Register>((raw >> 15) & 0x1F);
    this->rs2 = static_cast<Register>((raw >> 20) & 0x1F);

    this->imm = (((raw >> 31) & 0x01) << 12) | (((raw >> 7) & 0x01) << 11) |
                (((raw >> 25) & 0x3F) << 5) | (((raw >> 8) & 0x0F) << 1);

    if (this->instr != Operator::bltu && this->instr != Operator::bgeu)
        this->imm = (this->imm << 19) >> 19;
}

const std::string& BInstruction::toString() {
    if (this->printOut != "") return this->printOut;

    std::string symbol;

    switch (this->instr) {
        case Operator::beq:
            symbol = "==";
            break;
        case Operator::bne:
            symbol = "!=";
            break;
        case Operator::blt:
            symbol = "<";
            break;
        case Operator::bge:
            symbol = ">=";
            break;
        case Operator::bltu:
            symbol = "<";
            break;
        case Operator::bgeu:
            symbol = ">=";
            break;
        default:
            break;
    }

    this->printOut =
        "\t" + to_string(this->instr) + " " + to_string(this->rs1) + ", " +
        to_string(this->rs2) + ", " + std::to_string(this->imm) + "\t\t# if(" +
        to_string(this->rs1) + " " + symbol + " " + to_string(this->rs2) +
        ") PC += " + std::to_string(this->imm);

    return this->printOut;
}

UInstruction::UInstruction(Opcode op, uint32_t raw, uint32_t addr)
    : RiscvInstruction(op), addr(addr) {
    this->instr = (op == Opcode::LUI) ? Operator::lui : Operator::auipc;
    this->rd = static_cast<Register>((raw >> 7) & 0x1F);
    this->imm = (raw >> 12);
}

const std::string& UInstruction::toString() {
    if (this->printOut != "") return this->printOut;

    this->printOut = "\t" + to_string(this->instr) + " " + to_string(this->rd) +
                     ", " + std::to_string(this->imm) + "\t\t# " +
                     to_string(this->rd) + " = ";

    // Cannot be an unknown opcode as it will have been caught earlier
    if (this->op == Opcode::LUI) {
        this->printOut += std::to_string(this->imm) + " << 12";
    } else {
        this->printOut += "PC + (" + std::to_string(this->imm) + " << 12)";
    }

    return this->printOut;
}

JInstruction::JInstruction(Opcode op, uint32_t raw) : RiscvInstruction(op) {
    this->instr = Operator::jal;
    this->rd = static_cast<Register>((raw >> 7) & 0x1F);
    this->imm = (((raw >> 31) & 0x1) << 20) | (((raw >> 12) & 0xFF) << 12) |
                (((raw >> 20) & 0x1) << 11) | (((raw >> 21) & 0x3FF) << 1);

    this->imm = (this->imm << 11) >> 11;
}

const std::string& JInstruction::toString() {
    if (this->printOut != "") return this->printOut;

    this->printOut = "\t" + to_string(this->instr) + " " + to_string(this->rd) +
                     ", " + std::to_string(this->imm) + "\t\t# " +
                     to_string(this->rd) +
                     " = PC+$; PC += " + std::to_string(this->imm);

    return this->printOut;
}

const std::string& PseudoLoadInstruction::toString() {
    if (this->printOut != "") return this->printOut;

    this->printOut = "\t" + to_string(instr) + " " + to_string(rd) + ", " +
                     symbol + "\t\t# ";

    if (instr == Operator::la) {
        this->printOut += to_string(rd) + " = " + "&" + symbol;
    } else {
        this->printOut += to_string(rd) + " = M[&" + symbol + "]";
    }

    return this->printOut;
}

const std::string& PseudoStoreInstruction::toString() {
    if (this->printOut != "") return this->printOut;

    this->printOut = "\t" + to_string(instr) + " " + to_string(rd) + ", " +
                     symbol + ", " + to_string(rt) + "\t\t# M[&" + symbol +
                     "] = " + to_string(rt);

    if (instr == Operator::sb) {
        this->printOut += "[7:0]";
    } else if (instr == Operator::sh) {
        this->printOut += "[15:0]";
    }

    return this->printOut;
}

const std::string& EntryPoint::toString() {
    if (this->printOut != "") return this->printOut;

    printOut = "\n" + name + ":";

    return printOut;
}

const std::string& JInstructionEntry::toString() {
    if (this->printOut != "") return this->printOut;

    printOut = this->printOut = "\t" + to_string(this->instr) + " " +
                                to_string(this->rd) + ", " + entryPoint +
                                "\t\t# " + to_string(this->rd) +
                                " = PC+$; PC = &" + entryPoint;

    return this->printOut;
}

const std::string& BInstructionEntry::toString() {
    if (this->printOut != "") return this->printOut;

    std::string symbol;

    switch (this->instr) {
        case Operator::beq:
            symbol = "==";
            break;
        case Operator::bne:
            symbol = "!=";
            break;
        case Operator::blt:
            symbol = "<";
            break;
        case Operator::bge:
            symbol = ">=";
            break;
        case Operator::bltu:
            symbol = "<";
            break;
        case Operator::bgeu:
            symbol = ">=";
            break;
        default:
            break;
    }

    this->printOut = "\t" + to_string(this->instr) + " " +
                     to_string(this->rs1) + ", " + to_string(this->rs2) + ", " +
                     entryPoint + "\t\t# if(" + to_string(this->rs1) + " " +
                     symbol + " " + to_string(this->rs2) + ") PC = &" +
                     entryPoint;

    return this->printOut;
}
}  // namespace Disassembler