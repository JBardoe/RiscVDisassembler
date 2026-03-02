#include "disassembler/RiscvInstruction.hpp"

#include <iostream>

#include "disassembler/RiscvTypes.hpp"
#include "translator/ArmTypes.hpp"
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

std::vector<Translator::ArmInstruction> RInstruction::toArm() {
    Translator::Register wd = static_cast<Translator::Register>(rd);
    Translator::Register wn = static_cast<Translator::Register>(rs1);
    Translator::Register wm = static_cast<Translator::Register>(rs2);

    switch (instr) {
        case Disassembler::Operator::add:
            return {Translator::RRRInstruction(Translator::Operator::add, wd,
                                               wn, wm)};
        case Disassembler::Operator::sub:
            return {Translator::RRRInstruction(Translator::Operator::sub, wd,
                                               wn, wm)};
        case Disassembler::Operator::Xor:
            return {Translator::RRRInstruction(Translator::Operator::eor, wd,
                                               wn, wm)};
        case Disassembler::Operator::Or:
            return {Translator::RRRInstruction(Translator::Operator::orr, wd,
                                               wn, wm)};
        case Disassembler::Operator::And:
            return {Translator::RRRInstruction(Translator::Operator::And, wd,
                                               wn, wm)};
        case Disassembler::Operator::sll:
            return {Translator::RRRInstruction(Translator::Operator::lsl, wd,
                                               wn, wm)};
        case Disassembler::Operator::srl:
            return {Translator::RRRInstruction(Translator::Operator::lsr, wd,
                                               wn, wm)};
        case Disassembler::Operator::sra:
            return {Translator::RRRInstruction(Translator::Operator::asr, wd,
                                               wn, wm)};
        case Disassembler::Operator::slt:
            return {
                Translator::RRInstruction(Translator::Operator::cmp, wn, wm),
                Translator::RRInstruction(Translator::Operator::cset, wd,
                                          Translator::Register::lt)};
        case Disassembler::Operator::sltu:
            return {
                Translator::RRInstruction(Translator::Operator::cmp, wn, wm),
                Translator::RRInstruction(Translator::Operator::cset, wd,
                                          Translator::Register::lo)};
    }

    return {};
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

std::vector<Translator::ArmInstruction> IInstruction::toArm() {
    Translator::Register wd = static_cast<Translator::Register>(rd);
    Translator::Register wn = static_cast<Translator::Register>(rs1);

    switch (instr) {
        case Operator::addi:
            return {Translator::RRIInstruction(Translator::Operator::add, wd,
                                               wn, imm, false)};
        case Operator::xori:
            return {Translator::RRIInstruction(Translator::Operator::eor, wd,
                                               wn, imm, false)};
        case Operator::ori:
            return {Translator::RRIInstruction(Translator::Operator::orr, wd,
                                               wn, imm, false)};
        case Operator::andi:
            return {Translator::RRIInstruction(Translator::Operator::And, wd,
                                               wn, imm, false)};
        case Operator::slli:
            return {Translator::RRIInstruction(Translator::Operator::lsl, wd,
                                               wn, imm, false)};
        case Operator::srli:
            return {Translator::RRIInstruction(Translator::Operator::lsr, wd,
                                               wn, imm, false)};
        case Operator::srai:
            return {Translator::RRIInstruction(Translator::Operator::asr, wd,
                                               wn, imm, false)};
        case Operator::slti:
            return {
                Translator::RIInstruction(Translator::Operator::cmp, wn, imm),
                Translator::RRInstruction(Translator::Operator::cset, wd,
                                          Translator::Register::lt)};
        case Operator::sltiu:
            return {
                Translator::RIInstruction(Translator::Operator::cmp, wn, imm),
                Translator::RRInstruction(Translator::Operator::cset, wd,
                                          Translator::Register::lo)};
        case Operator::lb:
            return {Translator::RRIInstruction(Translator::Operator::ldrsb, wd,
                                               wn, imm, false)};
        case Operator::lh:
            return {Translator::RRIInstruction(Translator::Operator::ldrsh, wd,
                                               wn, imm, false)};
        case Operator::lw:
            return {Translator::RRIInstruction(Translator::Operator::ldrsw, wd,
                                               wn, imm, false)};
        case Operator::lbu:
            return {Translator::RRIInstruction(Translator::Operator::ldrb, wd,
                                               wn, imm, false)};
        case Operator::lhu:
            return {Translator::RRIInstruction(Translator::Operator::ldrh, wd,
                                               wn, imm, false)};
        case Operator::ecall:
            return {Translator::EInstruction(Translator::Operator::svc)};
        case Operator::ebreak:
            return {Translator::EInstruction(Translator::Operator::brk)};
    }

    // jalr
    if (rd == Register::ra) {
        return {Translator::RRIInstruction(Translator::Operator::add,
                                           Translator::Register::w15, wn, imm,
                                           false),
                Translator::BRInstruction(Translator::Operator::blr,
                                          Translator::Register::w15)};
    } else if (rd == Register::zero) {
        return {Translator::RRIInstruction(Translator::Operator::add,
                                           Translator::Register::w15, wn, imm,
                                           false),
                Translator::BRInstruction(Translator::Operator::br,
                                          Translator::Register::w15)};
    }

    return {
        Translator::RRIInstruction(Translator::Operator::add,
                                   Translator::Register::w15, wn, imm, false),
        Translator::RIInstruction(Translator::Operator::adr, wd, 8),
        Translator::BRInstruction(Translator::Operator::br,
                                  Translator::Register::w15)};
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

std::vector<Translator::ArmInstruction> SInstruction::toArm() {
    Translator::Register wd = static_cast<Translator::Register>(rs1);
    Translator::Register wn = static_cast<Translator::Register>(rs2);

    switch (instr) {
        case Operator::sb:
            return {Translator::RRIInstruction(Translator::Operator::strb, wd,
                                               wn, imm, false)};
        case Operator::sh:
            return {Translator::RRIInstruction(Translator::Operator::strh, wd,
                                               wn, imm, false)};
        case Operator::sw:
            return {Translator::RRIInstruction(Translator::Operator::str, wd,
                                               wn, imm, false)};
    }

    return {};
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

std::vector<Translator::ArmInstruction> BInstruction::toArm() {
    Translator::Register wd = static_cast<Translator::Register>(rs1);
    Translator::Register wn = static_cast<Translator::Register>(rs2);

    switch (instr) {
        case Operator::beq:
            return {
                Translator::RRInstruction(Translator::Operator::cmp, wd, wn),
                Translator::BIInstruction(Translator::Operator::beq, imm)};
        case Operator::bne:
            return {
                Translator::RRInstruction(Translator::Operator::cmp, wd, wn),
                Translator::BIInstruction(Translator::Operator::bne, imm)};
        case Operator::blt:
            return {
                Translator::RRInstruction(Translator::Operator::cmp, wd, wn),
                Translator::BIInstruction(Translator::Operator::blt, imm)};
        case Operator::bge:
            return {
                Translator::RRInstruction(Translator::Operator::cmp, wd, wn),
                Translator::BIInstruction(Translator::Operator::bge, imm)};
        case Operator::bltu:
            return {
                Translator::RRInstruction(Translator::Operator::cmp, wd, wn),
                Translator::BIInstruction(Translator::Operator::blo, imm)};
        case Operator::bgeu:
            return {
                Translator::RRInstruction(Translator::Operator::cmp, wd, wn),
                Translator::BIInstruction(Translator::Operator::bhs, imm)};
    }

    return {};
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

std::vector<Translator::ArmInstruction> UInstruction::toArm() {
    Translator::Register wd = static_cast<Translator::Register>(rd);

    if (instr == Operator::lui) {
        return {
            Translator::RIInstruction(Translator::Operator::movz, wd, 0),
            Translator::RRIInstruction(Translator::Operator::add, wd, wd, imm,
                                       true),
        };
    }

    return {Translator::RIInstruction(Translator::Operator::adr, wd, 0),
            Translator::RRIInstruction(Translator::Operator::add, wd, wd, imm,
                                       true)};
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

std::vector<Translator::ArmInstruction> JInstruction::toArm() {
    if (rd == Register::ra) {
        return {Translator::BIInstruction(Translator::Operator::bl, imm)};
    } else if (rd == Register::zero) {
        return {Translator::BIInstruction(Translator::Operator::b, imm)};
    }

    return {Translator::RIInstruction(Translator::Operator::adr,
                                      static_cast<Translator::Register>(rd), 8),
            Translator::BIInstruction(Translator::Operator::b, imm)};
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

std::vector<Translator::ArmInstruction> PseudoLoadInstruction::toArm() {
    Translator::Register wd = static_cast<Translator::Register>(rd);

    switch (instr) {
        case Operator::la:
            return {Translator::RSInstruction(Translator::Operator::adr, wd,
                                              symbol)};
        case Operator::lb:
            return {Translator::RSInstruction(Translator::Operator::ldrsb, wd,
                                              symbol)};
        case Operator::lh:
            return {Translator::RSInstruction(Translator::Operator::ldrsh, wd,
                                              symbol)};
        case Operator::lw:
            return {Translator::RSInstruction(Translator::Operator::ldrsw, wd,
                                              symbol)};
    }

    return {};
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

std::vector<Translator::ArmInstruction> PseudoStoreInstruction::toArm() {
    Translator::Register wt = static_cast<Translator::Register>(rd);

    switch (instr) {
        case Operator::sb:
            return {Translator::RSInstruction(Translator::Operator::strb, wt,
                                              symbol)};
        case Operator::sh:
            return {Translator::RSInstruction(Translator::Operator::strh, wt,
                                              symbol)};
        case Operator::sw:
            return {Translator::RSInstruction(Translator::Operator::str, wt,
                                              symbol)};
    }

    return {};
}

const std::string& EntryPoint::toString() {
    if (this->printOut != "") return this->printOut;

    printOut = "\n" + name + ":";

    return printOut;
}

std::vector<Translator::ArmInstruction> EntryPoint::toArm() {
    return {Translator::EntryPoint(name)};
}

const std::string& JInstructionEntry::toString() {
    if (this->printOut != "") return this->printOut;

    printOut = this->printOut = "\t" + to_string(this->instr) + " " +
                                to_string(this->rd) + ", " + entryPoint +
                                "\t\t# " + to_string(this->rd) +
                                " = PC+$; PC = &" + entryPoint;

    return this->printOut;
}

std::vector<Translator::ArmInstruction> JInstructionEntry::toArm() {
    if (rd == Register::ra) {
        return {
            Translator::BLInstruction(Translator::Operator::bl, entryPoint)};
    } else if (rd == Register::zero) {
        return {Translator::BLInstruction(Translator::Operator::b, entryPoint)};
    }

    return {Translator::RIInstruction(Translator::Operator::adr,
                                      static_cast<Translator::Register>(rd), 8),
            Translator::BLInstruction(Translator::Operator::b, entryPoint)};
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

std::vector<Translator::ArmInstruction> BInstructionEntry::toArm() {
    Translator::Register wd = static_cast<Translator::Register>(rs1);
    Translator::Register wn = static_cast<Translator::Register>(rs2);

    switch (instr) {
        case Operator::beq:
            return {
                Translator::RRInstruction(Translator::Operator::cmp, wd, wn),
                Translator::BLInstruction(Translator::Operator::beq,
                                          entryPoint)};
        case Operator::bne:
            return {
                Translator::RRInstruction(Translator::Operator::cmp, wd, wn),
                Translator::BLInstruction(Translator::Operator::bne,
                                          entryPoint)};
        case Operator::blt:
            return {
                Translator::RRInstruction(Translator::Operator::cmp, wd, wn),
                Translator::BLInstruction(Translator::Operator::blt,
                                          entryPoint)};
        case Operator::bge:
            return {
                Translator::RRInstruction(Translator::Operator::cmp, wd, wn),
                Translator::BLInstruction(Translator::Operator::bge,
                                          entryPoint)};
        case Operator::bltu:
            return {
                Translator::RRInstruction(Translator::Operator::cmp, wd, wn),
                Translator::BLInstruction(Translator::Operator::blo,
                                          entryPoint)};
        case Operator::bgeu:
            return {
                Translator::RRInstruction(Translator::Operator::cmp, wd, wn),
                Translator::BLInstruction(Translator::Operator::bhs,
                                          entryPoint)};
    }

    return {};
}
}  // namespace Disassembler