#include "disassembler/RiscvInstruction.hpp"

#include <iomanip>
#include <iostream>

#include "translator/ArmTypes.hpp"
#include "utils/AssemblyTypes.hpp"
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

    // Raw register addresses have to be in the range 0-31 as it is parsed from
    // a 5 bit number
    this->rd = static_cast<Register>((raw >> 7) & 0x1F);
    this->rs1 = static_cast<Register>((raw >> 15) & 0x1F);
    this->rs2 = static_cast<Register>((raw >> 20) & 0x1F);
}

const std::string& RInstruction::toString() {
    if (this->printOut != "") return this->printOut;

    std::string symbol;

    this->printOut = "\t" + to_string(this->instr) + " " + to_string(this->rd) +
                     ", " + to_string(this->rs1) + ", " + to_string(this->rs2);

    if (this->printOut.size() < Assembly::COMMENT_COL)
        this->printOut.append(Assembly::COMMENT_COL - this->printOut.size(),
                              ' ');

    this->printOut += "# " + to_string(this->rd) + " = ";

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

std::vector<std::unique_ptr<Translator::ArmInstruction>> RInstruction::toArm() {
    Translator::Register wd = static_cast<Translator::Register>(rd);
    Translator::Register wn = static_cast<Translator::Register>(rs1);
    Translator::Register wm = static_cast<Translator::Register>(rs2);

    std::vector<std::unique_ptr<Translator::ArmInstruction>> ret;

    switch (instr) {
        case Operator::add:
            ret.push_back(std::make_unique<Translator::RRRInstruction>(
                Translator::Operator::add, wd, wn, wm));
            break;
        case Operator::sub:
            ret.push_back(std::make_unique<Translator::RRRInstruction>(
                Translator::Operator::sub, wd, wn, wm));
            break;
        case Operator::Xor:
            ret.push_back(std::make_unique<Translator::RRRInstruction>(
                Translator::Operator::eor, wd, wn, wm));
            break;
        case Operator::Or:
            ret.push_back(std::make_unique<Translator::RRRInstruction>(
                Translator::Operator::orr, wd, wn, wm));
            break;
        case Operator::And:
            ret.push_back(std::make_unique<Translator::RRRInstruction>(
                Translator::Operator::And, wd, wn, wm));
            break;
        case Operator::sll:
            ret.push_back(std::make_unique<Translator::RRRInstruction>(
                Translator::Operator::lsl, wd, wn, wm));
            break;
        case Operator::srl:
            ret.push_back(std::make_unique<Translator::RRRInstruction>(
                Translator::Operator::lsr, wd, wn, wm));
            break;
        case Operator::sra:
            ret.push_back(std::make_unique<Translator::RRRInstruction>(
                Translator::Operator::asr, wd, wn, wm));
            break;
        case Operator::slt:
            ret.push_back(std::make_unique<Translator::RRInstruction>(
                Translator::Operator::cmp, wn, wm));

            ret.push_back(std::make_unique<Translator::RRInstruction>(
                Translator::Operator::cset, wd, Translator::Register::lt));
            break;
        case Operator::sltu:
            ret.push_back(std::make_unique<Translator::RRInstruction>(
                Translator::Operator::cmp, wn, wm));
            ret.push_back(std::make_unique<Translator::RRInstruction>(
                Translator::Operator::cset, wd, Translator::Register::lo));
            break;
        default:
            break;
    }

    return ret;
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
                         to_string(rs1) + ")";

        if (this->printOut.size() < Assembly::COMMENT_COL)
            this->printOut.append(Assembly::COMMENT_COL - this->printOut.size(),
                                  ' ');

        this->printOut += "# " + to_string(this->rd) +
                          " = PC+4; PC = " + to_string(this->rs1) + " + " +
                          std::to_string(this->imm);

        return this->printOut;
    }

    if (this->instr == Operator::ecall) {
        this->printOut = "\tecall";

        if (this->printOut.size() < Assembly::COMMENT_COL)
            this->printOut.append(Assembly::COMMENT_COL - this->printOut.size(),
                                  ' ');

        this->printOut += "# Transfer control to OS";
        return this->printOut;
    }

    if (this->instr == Operator::ebreak) {
        this->printOut = "\tebreak";

        if (this->printOut.size() < Assembly::COMMENT_COL)
            this->printOut.append(Assembly::COMMENT_COL - this->printOut.size(),
                                  ' ');

        this->printOut += "# Transfer control to debugger";
        return this->printOut;
    }

    if (this->op == Opcode::LOAD) {
        this->printOut =
            "\t" + to_string(this->instr) + " " + to_string(this->rd) + ", " +
            std::to_string(this->imm) + "(" + to_string(this->rs1) + ")";

        if (this->printOut.size() < Assembly::COMMENT_COL)
            this->printOut.append(Assembly::COMMENT_COL - this->printOut.size(),
                                  ' ');

        this->printOut += "# " + to_string(this->rd) + " = M[" +
                          to_string(this->rs1) + "+" +
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
                     std::to_string(this->imm);

    if (this->printOut.size() < Assembly::COMMENT_COL)
        this->printOut.append(Assembly::COMMENT_COL - this->printOut.size(),
                              ' ');

    this->printOut += "# " + to_string(this->rd) + " = ";

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

std::vector<std::unique_ptr<Translator::ArmInstruction>> IInstruction::toArm() {
    Translator::Register wd = static_cast<Translator::Register>(rd);
    Translator::Register wn = static_cast<Translator::Register>(rs1);

    std::vector<std::unique_ptr<Translator::ArmInstruction>> ret;

    switch (instr) {
        case Operator::addi:
            ret.push_back(std::make_unique<Translator::RRIInstruction>(
                Translator::Operator::add, wd, wn, imm, false));
            break;
        case Operator::xori:
            ret.push_back(std::make_unique<Translator::RRIInstruction>(
                Translator::Operator::eor, wd, wn, imm, false));
            break;
        case Operator::ori:
            ret.push_back(std::make_unique<Translator::RRIInstruction>(
                Translator::Operator::orr, wd, wn, imm, false));
            break;
        case Operator::andi:
            ret.push_back(std::make_unique<Translator::RRIInstruction>(
                Translator::Operator::And, wd, wn, imm, false));
            break;
        case Operator::slli:
            ret.push_back(std::make_unique<Translator::RRIInstruction>(
                Translator::Operator::lsl, wd, wn, imm, false));
            break;
        case Operator::srli:
            ret.push_back(std::make_unique<Translator::RRIInstruction>(
                Translator::Operator::lsr, wd, wn, imm, false));
            break;
        case Operator::srai:
            ret.push_back(std::make_unique<Translator::RRIInstruction>(
                Translator::Operator::asr, wd, wn, imm, false));
            break;
        case Operator::slti:
            ret.push_back(std::make_unique<Translator::RIInstruction>(
                Translator::Operator::cmp, wn, imm));
            ret.push_back(std::make_unique<Translator::RRInstruction>(
                Translator::Operator::cset, wd, Translator::Register::lt));
            break;
        case Operator::sltiu:
            ret.push_back(std::make_unique<Translator::RIInstruction>(
                Translator::Operator::cmp, wn, imm));

            ret.push_back(std::make_unique<Translator::RRInstruction>(
                Translator::Operator::cset, wd, Translator::Register::lo));
            break;
        case Operator::lb:
            ret.push_back(std::make_unique<Translator::RRIInstruction>(
                Translator::Operator::ldrsb, wd, wn, imm, false));
            break;
        case Operator::lh:
            ret.push_back(std::make_unique<Translator::RRIInstruction>(
                Translator::Operator::ldrsh, wd, wn, imm, false));
            break;
        case Operator::lw:
            ret.push_back(std::make_unique<Translator::RRIInstruction>(
                Translator::Operator::ldr, wd, wn, imm, false));
            break;
        case Operator::lbu:
            ret.push_back(std::make_unique<Translator::RRIInstruction>(
                Translator::Operator::ldrb, wd, wn, imm, false));
            break;
        case Operator::lhu:
            ret.push_back(std::make_unique<Translator::RRIInstruction>(
                Translator::Operator::ldrh, wd, wn, imm, false));
            break;
        case Operator::ecall:
            ret.push_back(std::make_unique<Translator::EInstruction>(
                Translator::Operator::svc));
            break;
        case Operator::ebreak:
            ret.push_back(std::make_unique<Translator::EInstruction>(
                Translator::Operator::brk));
            break;
        default:
            break;
    }

    if (!ret.empty()) return ret;

    // jalr
    if (rd == Register::ra) {
        ret.push_back(std::make_unique<Translator::RRIInstruction>(
            Translator::Operator::add, Translator::Register::w15, wn, imm,
            false));
        ret.push_back(std::make_unique<Translator::BRInstruction>(
            Translator::Operator::blr, Translator::Register::w15));
    } else if (rd == Register::zero) {
        ret.push_back(std::make_unique<Translator::RRIInstruction>(
            Translator::Operator::add, Translator::Register::w15, wn, imm,
            false));
        ret.push_back(std::make_unique<Translator::BRInstruction>(
            Translator::Operator::br, Translator::Register::w15));
    } else {
        ret.push_back(std::make_unique<Translator::RRIInstruction>(
            Translator::Operator::add, Translator::Register::w15, wn, imm,
            false));
        ret.push_back(std::make_unique<Translator::RIInstruction>(
            Translator::Operator::adr, wd, 8));
        ret.push_back(std::make_unique<Translator::BRInstruction>(
            Translator::Operator::br, Translator::Register::w15));
    }

    return ret;
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

    this->printOut = "\t" + to_string(this->instr) + " " +
                     to_string(this->rs2) + ", " + std::to_string(this->imm) +
                     "(" + to_string(this->rs1) + ")";

    if (this->printOut.size() < Assembly::COMMENT_COL)
        this->printOut.append(Assembly::COMMENT_COL - this->printOut.size(),
                              ' ');

    this->printOut +=
        "# M[" + to_string(this->rs1) + "+" + std::to_string(this->imm) +
        "][0:" + std::to_string(upper) + "] = " + to_string(this->rs2) +
        "[0:" + std::to_string(upper) + "]";

    return this->printOut;
}

std::vector<std::unique_ptr<Translator::ArmInstruction>> SInstruction::toArm() {
    Translator::Register wd = static_cast<Translator::Register>(rs1);
    Translator::Register wn = static_cast<Translator::Register>(rs2);

    std::vector<std::unique_ptr<Translator::ArmInstruction>> ret;

    switch (instr) {
        case Operator::sb:
            ret.push_back(std::make_unique<Translator::RRIInstruction>(
                Translator::Operator::strb, wd, wn, imm, false));
            break;
        case Operator::sh:
            ret.push_back(std::make_unique<Translator::RRIInstruction>(
                Translator::Operator::strh, wd, wn, imm, false));
            break;
        case Operator::sw:
            ret.push_back(std::make_unique<Translator::RRIInstruction>(
                Translator::Operator::str, wd, wn, imm, false));
            break;
        default:
            break;
    }

    return ret;
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

    if (this->imm % 4 != 0) {
        throw DisassemblyException("Branch offset not divisible by 4");
    }

    this->dir = static_cast<Assembly::BranchDirection>(imm < 0);
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

    this->printOut = "\t" + to_string(this->instr) + " " +
                     to_string(this->rs1) + ", " + to_string(this->rs2) + ", " +
                     std::to_string(this->imm);

    if (this->printOut.size() < Assembly::COMMENT_COL)
        this->printOut.append(Assembly::COMMENT_COL - this->printOut.size(),
                              ' ');

    this->printOut += "# if(" + to_string(this->rs1) + " " + symbol + " " +
                      to_string(this->rs2) +
                      ") PC += " + std::to_string(this->imm);

    return this->printOut;
}

std::vector<std::unique_ptr<Translator::ArmInstruction>> BInstruction::toArm() {
    Translator::Register wd = static_cast<Translator::Register>(rs1);
    Translator::Register wn = static_cast<Translator::Register>(rs2);

    std::vector<std::unique_ptr<Translator::ArmInstruction>> ret;

    switch (instr) {
        case Operator::beq:
            ret.push_back(std::make_unique<Translator::RRInstruction>(
                Translator::Operator::cmp, wd, wn));
            ret.push_back(std::make_unique<Translator::BIInstruction>(
                Translator::Operator::beq, imm));
            break;
        case Operator::bne:
            ret.push_back(std::make_unique<Translator::RRInstruction>(
                Translator::Operator::cmp, wd, wn));
            ret.push_back(std::make_unique<Translator::BIInstruction>(
                Translator::Operator::bne, imm));
            break;
        case Operator::blt:
            ret.push_back(std::make_unique<Translator::RRInstruction>(
                Translator::Operator::cmp, wd, wn));
            ret.push_back(std::make_unique<Translator::BIInstruction>(
                Translator::Operator::blt, imm));
            break;
        case Operator::bge:
            ret.push_back(std::make_unique<Translator::RRInstruction>(
                Translator::Operator::cmp, wd, wn));
            ret.push_back(std::make_unique<Translator::BIInstruction>(
                Translator::Operator::bge, imm));
            break;
        case Operator::bltu:
            ret.push_back(std::make_unique<Translator::RRInstruction>(
                Translator::Operator::cmp, wd, wn));
            ret.push_back(std::make_unique<Translator::BIInstruction>(
                Translator::Operator::blo, imm));
            break;
        case Operator::bgeu:
            ret.push_back(std::make_unique<Translator::RRInstruction>(
                Translator::Operator::cmp, wd, wn));
            ret.push_back(std::make_unique<Translator::BIInstruction>(
                Translator::Operator::bhs, imm));
            break;
        default:
            break;
    }

    return ret;
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
                     ", " + std::to_string(this->imm);

    if (this->printOut.size() < Assembly::COMMENT_COL)
        this->printOut.append(Assembly::COMMENT_COL - this->printOut.size(),
                              ' ');

    this->printOut += "# " + to_string(this->rd) + " = ";

    // Cannot be an unknown opcode as it will have been caught earlier
    if (this->op == Opcode::LUI) {
        this->printOut += std::to_string(this->imm) + " << 12";
    } else {
        this->printOut += "PC + (" + std::to_string(this->imm) + " << 12)";
    }

    return this->printOut;
}

std::vector<std::unique_ptr<Translator::ArmInstruction>> UInstruction::toArm() {
    Translator::Register wd = static_cast<Translator::Register>(rd);
    std::vector<std::unique_ptr<Translator::ArmInstruction>> ret;

    if (instr == Operator::lui) {
        ret.push_back(std::make_unique<Translator::RIInstruction>(
            Translator::Operator::movz, wd, 0));

        ret.push_back(std::make_unique<Translator::RRIInstruction>(
            Translator::Operator::add, wd, wd, imm, true));

    } else {
        ret.push_back(std::make_unique<Translator::RIInstruction>(
            Translator::Operator::adr, wd, 0));

        ret.push_back(std::make_unique<Translator::RRIInstruction>(
            Translator::Operator::add, wd, wd, imm, true));
    }

    return ret;
}

JInstruction::JInstruction(Opcode op, uint32_t raw) : RiscvInstruction(op) {
    this->instr = Operator::jal;
    this->rd = static_cast<Register>((raw >> 7) & 0x1F);
    this->imm = (((raw >> 31) & 0x1) << 20) | (((raw >> 12) & 0xFF) << 12) |
                (((raw >> 20) & 0x1) << 11) | (((raw >> 21) & 0x3FF) << 1);

    this->imm = (this->imm << 11) >> 11;

    if (this->imm % 4 != 0) {
        throw DisassemblyException("Jump offset not divisible by 4");
    }
}

const std::string& JInstruction::toString() {
    if (this->printOut != "") return this->printOut;

    this->printOut = "\t" + to_string(this->instr) + " " + to_string(this->rd) +
                     ", " + std::to_string(this->imm);

    if (this->printOut.size() < Assembly::COMMENT_COL)
        this->printOut.append(Assembly::COMMENT_COL - this->printOut.size(),
                              ' ');

    this->printOut += "# " + to_string(this->rd) +
                      " = PC+4; PC += " + std::to_string(this->imm);

    return this->printOut;
}

std::vector<std::unique_ptr<Translator::ArmInstruction>> JInstruction::toArm() {
    std::vector<std::unique_ptr<Translator::ArmInstruction>> ret;

    if (rd == Register::ra) {
        ret.push_back(std::make_unique<Translator::BIInstruction>(
            Translator::Operator::bl, imm));
    } else if (rd == Register::zero) {
        ret.push_back(std::make_unique<Translator::BIInstruction>(
            Translator::Operator::b, imm));
    } else {
        ret.push_back(std::make_unique<Translator::RIInstruction>(
            Translator::Operator::adr, static_cast<Translator::Register>(rd),
            8));
        ret.push_back(std::make_unique<Translator::BIInstruction>(
            Translator::Operator::b, imm));
    }

    return ret;
}

const std::string& PseudoLoadInstruction::toString() {
    if (this->printOut != "") return this->printOut;

    this->printOut =
        "\t" + to_string(instr) + " " + to_string(rd) + ", " + symbol;

    if (this->printOut.size() < Assembly::COMMENT_COL)
        this->printOut.append(Assembly::COMMENT_COL - this->printOut.size(),
                              ' ');

    this->printOut += "# ";

    if (instr == Operator::la) {
        this->printOut += to_string(rd) + " = " + "&" + symbol;
    } else {
        this->printOut += to_string(rd) + " = M[&" + symbol + "]";
    }

    return this->printOut;
}

std::vector<std::unique_ptr<Translator::ArmInstruction>>
PseudoLoadInstruction::toArm() {
    Translator::Register wd = static_cast<Translator::Register>(rd);

    std::vector<std::unique_ptr<Translator::ArmInstruction>> ret;

    switch (instr) {
        case Operator::la:
            ret.push_back(std::make_unique<Translator::RSInstruction>(
                Translator::Operator::adr, wd, symbol));
            break;
        case Operator::lb:
            ret.push_back(std::make_unique<Translator::RSInstruction>(
                Translator::Operator::ldrsb, wd, symbol));
            break;
        case Operator::lh:
            ret.push_back(std::make_unique<Translator::RSInstruction>(
                Translator::Operator::ldrsh, wd, symbol));
            break;
        case Operator::lw:
            ret.push_back(std::make_unique<Translator::RSInstruction>(
                Translator::Operator::ldr, wd, symbol));
            break;
        default:
            break;
    }

    return ret;
}

const std::string& PseudoStoreInstruction::toString() {
    if (this->printOut != "") return this->printOut;

    this->printOut = "\t" + to_string(instr) + " " + to_string(rd) + ", " +
                     symbol + ", " + to_string(rt);

    if (this->printOut.size() < Assembly::COMMENT_COL)
        this->printOut.append(Assembly::COMMENT_COL - this->printOut.size(),
                              ' ');

    this->printOut += "# M[&" + symbol + "] = " + to_string(rt);

    if (instr == Operator::sb) {
        this->printOut += "[7:0]";
    } else if (instr == Operator::sh) {
        this->printOut += "[15:0]";
    }

    return this->printOut;
}

std::vector<std::unique_ptr<Translator::ArmInstruction>>
PseudoStoreInstruction::toArm() {
    Translator::Register wt = static_cast<Translator::Register>(rd);
    std::vector<std::unique_ptr<Translator::ArmInstruction>> ret;

    switch (instr) {
        case Operator::sb:
            ret.push_back(std::make_unique<Translator::RSInstruction>(
                Translator::Operator::strb, wt, symbol));
            break;
        case Operator::sh:
            ret.push_back(std::make_unique<Translator::RSInstruction>(
                Translator::Operator::strh, wt, symbol));
            break;
        case Operator::sw:
            ret.push_back(std::make_unique<Translator::RSInstruction>(
                Translator::Operator::str, wt, symbol));
            break;
        default:
            break;
    }

    return ret;
}

const std::string& EntryPoint::toString() {
    if (this->printOut != "") return this->printOut;

    printOut = "\n" + name + ":";

    return printOut;
}

std::vector<std::unique_ptr<Translator::ArmInstruction>> EntryPoint::toArm() {
    std::vector<std::unique_ptr<Translator::ArmInstruction>> ret;
    ret.push_back(std::make_unique<Translator::EntryPoint>(name));
    return ret;
}

const std::string& JInstructionEntry::toString() {
    if (this->printOut != "") return this->printOut;

    printOut = this->printOut = "\t" + to_string(this->instr) + " " +
                                to_string(this->rd) + ", " + entryPoint;

    if (this->printOut.size() < Assembly::COMMENT_COL)
        this->printOut.append(Assembly::COMMENT_COL - this->printOut.size(),
                              ' ');

    this->printOut +=
        "# " + to_string(this->rd) + " = PC+4; PC = &" + entryPoint;

    return this->printOut;
}

std::vector<std::unique_ptr<Translator::ArmInstruction>>
JInstructionEntry::toArm() {
    std::vector<std::unique_ptr<Translator::ArmInstruction>> ret;

    if (rd == Register::ra) {
        ret.push_back(std::make_unique<Translator::BLInstruction>(
            Translator::Operator::bl, entryPoint,
            Assembly::BranchDirection::JUMP));
    } else if (rd == Register::zero) {
        ret.push_back(std::make_unique<Translator::BLInstruction>(
            Translator::Operator::b, entryPoint,
            Assembly::BranchDirection::JUMP));
    } else {
        ret.push_back(std::make_unique<Translator::RIInstruction>(
            Translator::Operator::adr, static_cast<Translator::Register>(rd),
            8));
        ret.push_back(std::make_unique<Translator::BLInstruction>(
            Translator::Operator::b, entryPoint,
            Assembly::BranchDirection::JUMP));
    }

    return ret;
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
                     entryPoint;

    if (this->printOut.size() < Assembly::COMMENT_COL)
        this->printOut.append(Assembly::COMMENT_COL - this->printOut.size(),
                              ' ');

    this->printOut += "# if(" + to_string(this->rs1) + " " + symbol + " " +
                      to_string(this->rs2) + ") PC = &" + entryPoint;

    return this->printOut;
}

std::vector<std::unique_ptr<Translator::ArmInstruction>>
BInstructionEntry::toArm() {
    Translator::Register wd = static_cast<Translator::Register>(rs1);
    Translator::Register wn = static_cast<Translator::Register>(rs2);

    std::vector<std::unique_ptr<Translator::ArmInstruction>> ret;

    switch (instr) {
        case Operator::beq:
            ret.push_back(std::make_unique<Translator::RRInstruction>(
                Translator::Operator::cmp, wd, wn));
            ret.push_back(std::make_unique<Translator::BLInstruction>(
                Translator::Operator::beq, entryPoint, dir));
            break;
        case Operator::bne:
            ret.push_back(std::make_unique<Translator::RRInstruction>(
                Translator::Operator::cmp, wd, wn));
            ret.push_back(std::make_unique<Translator::BLInstruction>(
                Translator::Operator::bne, entryPoint, dir));
            break;
        case Operator::blt:
            ret.push_back(std::make_unique<Translator::RRInstruction>(
                Translator::Operator::cmp, wd, wn));
            ret.push_back(std::make_unique<Translator::BLInstruction>(
                Translator::Operator::blt, entryPoint, dir));
            break;
        case Operator::bge:
            ret.push_back(std::make_unique<Translator::RRInstruction>(
                Translator::Operator::cmp, wd, wn));
            ret.push_back(std::make_unique<Translator::BLInstruction>(
                Translator::Operator::bge, entryPoint, dir));
            break;
        case Operator::bltu:
            ret.push_back(std::make_unique<Translator::RRInstruction>(
                Translator::Operator::cmp, wd, wn));
            ret.push_back(std::make_unique<Translator::BLInstruction>(
                Translator::Operator::blo, entryPoint, dir));
            break;
        case Operator::bgeu:
            ret.push_back(std::make_unique<Translator::RRInstruction>(
                Translator::Operator::cmp, wd, wn));
            ret.push_back(std::make_unique<Translator::BLInstruction>(
                Translator::Operator::bhs, entryPoint, dir));
            break;
        default:
            break;
    }

    return ret;
}
}  // namespace Disassembler