#include "disassembler/Instruction.hpp"

#include "utils/DisassemblyException.hpp"

namespace Disassembler {

RInstruction::RInstruction(Opcode op, uint32_t raw) : op(op) {
    this->rd = (raw >> 7) & 0x1F;
    this->funct3 = (raw >> 12) & 0x07;
    this->rs1 = (raw >> 15) & 0x1F;
    this->rs2 = (raw >> 20) & 0x1F;
    this->funct7 = raw >> 25;
}

const std::string& RInstruction::toString() {
    if (this->printOut != "") return this->printOut;

    std::string symbol;

    this->printOut = "";

    switch (this->funct3) {
        case 0:
            this->printOut += (this->funct7) ? "sub " : "add ";
            symbol = (this->funct7) ? "-" : "+";
            break;
        case 1:
            this->printOut += "sll ";
            symbol = "<<";
            break;
        case 2:
            this->printOut += "slt ";
            break;
        case 3:
            this->printOut += "sltu ";
            break;
        case 4:
            this->printOut += "xor ";
            symbol = "^";
            break;
        case 5:
            this->printOut += (this->funct7) ? "sra " : "srl ";
            symbol = ">>";
            break;
        case 6:
            this->printOut += "or ";
            symbol = "|";
            break;
        case 7:
            this->printOut += "and ";
            symbol = "&";
            break;
        default:
            throw DisassemblyException(
                "Invalid funct3 parameter on R-Type instruction.");
    }

    // Registers to be in the range 0-31 as it is parsed from a 5 bit number
    this->printOut += std::string(registerNames[this->rd]) + " " +
                      std::string(registerNames[this->rs1]) + " " +
                      std::string(registerNames[this->rs2]);

    if (this->funct3 == 2 || this->funct3 == 3) {
        this->printOut += " # " + std::string(registerNames[this->rd]) +
                          " = (" + std::string(registerNames[this->rs1]) +
                          " < " + std::string(registerNames[this->rs2]) +
                          ")?1:0";
    } else {
        this->printOut += " # " + std::string(registerNames[this->rd]) + " = " +
                          std::string(registerNames[this->rs1]) + " " + symbol +
                          " " + std::string(registerNames[this->rs2]);
    }
    return this->printOut;
}

IInstruction::IInstruction(Opcode op, uint32_t raw) : op(op) {
    this->rd = (raw >> 7) & 0x1F;
    this->funct3 = (raw >> 12) & 0x07;
    this->rs1 = (raw >> 15) & 0x1F;
    this->imm = (raw >> 20);

    if ((this->op == Opcode::IMM_INSTR && this->funct3 != 3) ||
        (this->op == Opcode::LOAD && this->funct3 != 4 && this->funct3 != 5))
        this->imm = (this->imm << 20) >> 20;
}
const std::string& IInstruction::toString() {
    if (this->printOut != "") return this->printOut;

    if (this->op == Opcode::JALR) {
        this->printOut =
            "jalr " + std::string(registerNames[this->rd]) + ", " +
            std::to_string(this->imm) + "(" +
            std::string(registerNames[this->rs1]) + ") # " +
            std::string(registerNames[this->rd]) +
            " = PC+4; PC = " + std::string(registerNames[this->rs1]) + " + " +
            std::to_string(this->imm);
    } else if (this->op == Opcode::ENV_TYPE && this->imm == 0) {
        this->printOut = "ecall # Transfer control to OS";
    } else if (this->op == Opcode::ENV_TYPE && this->imm == 1) {
        this->printOut = "ebreak # Transfer control to debugger";
    } else if (this->op == Opcode::LOAD) {
        int upper;
        this->printOut = "";
        switch (this->funct3) {
            case 0:
                this->printOut += "lb ";
                upper = 7;
                break;
            case 1:
                this->printOut += "lh ";
                upper = 15;
                break;
            case 2:
                this->printOut += "lw ";
                upper = 31;
                break;
            case 4:
                this->printOut += "lbu ";
                upper = 7;
                break;
            case 5:
                this->printOut += "lhu ";
                upper = 15;
                break;
            default:
                throw DisassemblyException(
                    "Invalid funct3 parameter on I-Type instruction.");
        }

        this->printOut += std::string(registerNames[this->rd]) + ", " +
                          std::to_string(this->imm) + "(" +
                          std::string(registerNames[this->rs1]) + ") # " +
                          std::string(registerNames[this->rd]) + " = M[" +
                          std::string(registerNames[this->rs1]) + "+" +
                          std::to_string(this->imm) +
                          "][0:" + std::to_string(upper) + "]";
    } else {
        this->printOut = "";
        std::string symbol;
        switch (this->funct3) {
            case 0:
                this->printOut += "addi ";
                symbol = "+";
                break;
            case 1:
                this->printOut += "slli ";
                symbol = "<<";
                break;
            case 2:
                this->printOut += "slti ";
                break;
            case 3:
                this->printOut += "sltiu ";
                break;
            case 4:
                this->printOut += "xori ";
                symbol = "^";
                break;
            case 5:
                if ((imm >> 5) & 0x7F == 2) {
                    this->printOut += "srai ";
                } else if ((imm >> 5) & 0x7F == 0) {
                    this->printOut += "srli ";
                } else {
                    throw DisassemblyException(
                        "Invalid imm parameter on Shift Right instruction.");
                }
                symbol = ">>";
            case 6:
                this->printOut += "ori ";
                symbol = "|";
                break;
            case 7:
                this->printOut += "andi ";
                symbol = "&";
                break;
            default:
                throw DisassemblyException(
                    "Invalid funct3 parameter on I-Type instruction.");
        }

        this->printOut += std::string(registerNames[this->rd]) + " " +
                          std::string(registerNames[this->rs1]) + " " +
                          std::to_string(this->imm) + " # " +
                          std::string(registerNames[this->rd]) + " = ";

        if (this->funct3 == 2 || this->funct3 == 3) {
            this->printOut += "(" + std::string(registerNames[this->rs1]) +
                              " < " + std::to_string(this->imm) + ")?1:0";
        } else {
            this->printOut += std::string(registerNames[this->rs1]) + " " +
                              symbol + " " + std::to_string(this->imm);
        }

        if (this->funct3 == 1 || this->funct3 == 5) {
            this->printOut += "[0:4]";
        }
    }

    return this->printOut;
}

SInstruction::SInstruction(Opcode op, uint32_t raw) : op(op) {
    this->funct3 = (raw >> 12) & 0x07;
    this->rs1 = (raw >> 15) & 0x1F;
    this->rs2 = (raw >> 20) & 0x1F;
    this->imm = ((raw >> 7) & 0x17) | ((raw >> 25) << 5);

    this->imm = (this->imm << 20) >> 20;
}
const std::string& SInstruction::toString() {
    if (this->printOut != "") return this->printOut;

    this->printOut = "";

    int upper = 0;

    switch (this->funct3) {
        case 0:
            this->printOut += "sb ";
            upper = 7;
            break;
        case 1:
            this->printOut += "sh ";
            upper = 15;
            break;
        case 2:
            this->printOut += "sw ";
            upper = 31;
            break;
        default:
            throw DisassemblyException(
                "Invalid funct3 parameter on S-Type instruction.");
    }

    this->printOut += std::string(registerNames[this->rs2]) + ", " +
                      std::to_string(this->imm) + "(" +
                      std::string(registerNames[this->rs1]) + ") # M[" +
                      std::string(registerNames[this->rs1]) + "+" +
                      std::to_string(this->imm) +
                      "][0:" + std::to_string(upper) +
                      "] = " + std::string(registerNames[this->rs2]) +
                      "[0:" + std::to_string(upper) + "]";

    return this->printOut;
}

BInstruction::BInstruction(Opcode op, uint32_t raw) : op(op) {
    this->funct3 = (raw >> 12) & 0x07;
    this->rs1 = (raw >> 15) & 0x1F;
    this->rs2 = (raw >> 20) & 0x1F;
    this->imm = (((raw >> 31) & 0x01) << 12) | (((raw >> 7) & 0x01) << 11) |
                (((raw >> 25) & 0x3F) << 5) | (((raw >> 8) & 0x0F) << 1);

    if (this->funct3 != 6 && this->funct3 != 7)
        this->imm = (this->imm << 19) >> 19;
}
const std::string& BInstruction::toString() {
    if (this->printOut != "") return this->printOut;

    std::string symbol;

    this->printOut = "";

    switch (this->funct3) {
        case 0:
            this->printOut += "beq ";
            symbol = "==";
            break;
        case 1:
            this->printOut += "bne ";
            symbol = "!=";
            break;
        case 4:
            this->printOut += "blt ";
            symbol = "<";
            break;
        case 5:
            this->printOut += "bge ";
            symbol = ">=";
            break;
        case 6:
            this->printOut += "bltu ";
            symbol = "<";
            break;
        case 7:
            this->printOut += "bgeu ";
            symbol = ">=";
            break;
        default:
            throw DisassemblyException(
                "Invalid funct3 parameter on B-Type instruction.");
    }

    this->printOut += std::string(registerNames[this->rs1]) + " " +
                      std::string(registerNames[this->rs2]) + " " +
                      std::to_string(this->imm) + " # if(" +
                      std::string(registerNames[this->rs1]) + " " + symbol +
                      " " + std::string(registerNames[this->rs2]) +
                      ") PC += " + std::to_string(this->imm);

    return this->printOut;
}

UInstruction::UInstruction(Opcode op, uint32_t raw) : op(op) {
    this->rd = (raw >> 7) & 0x1F;
    this->imm = (raw >> 12);
}
const std::string& UInstruction::toString() {
    if (this->printOut != "") return this->printOut;

    // Cannot be an unknown opcode as it will have been caught earlier
    if (this->op == Opcode::LUI) {
        this->printOut = "lui " + std::string(registerNames[this->rd]) + " " +
                         std::to_string(this->imm) + " # " +
                         std::string(registerNames[this->rd]) + " = " +
                         std::to_string(this->imm) + " << 12";
    } else {
        this->printOut = "auipc " + std::string(registerNames[this->rd]) + " " +
                         std::to_string(this->imm) + " # " +
                         std::string(registerNames[this->rd]) + " = PC + (" +
                         std::to_string(this->imm) + " << 12)";
    }

    return this->printOut;
}

JInstruction::JInstruction(Opcode op, uint32_t raw) : op(op) {
    this->rd = (raw >> 7) & 0x1F;
    this->imm = (((raw >> 31) & 0x1) << 20) | (((raw >> 12) & 0xFF) << 12) |
                (((raw >> 20) & 0x1) << 11) | (((raw >> 21) & 0x3FF) << 1);

    this->imm = (this->imm << 11) >> 11;
}
const std::string& JInstruction::toString() {
    if (this->printOut != "") return this->printOut;

    this->printOut = "jal " + std::string(registerNames[this->rd]) + " " +
                     std::to_string(this->imm) + " # " +
                     std::string(registerNames[this->rd]) +
                     " = PC+$; PC += " + std::to_string(this->imm);

    return this->printOut;
}

}  // namespace Disassembler