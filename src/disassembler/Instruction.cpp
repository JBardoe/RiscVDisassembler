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

    if ((op == Opcode::IMM_INSTR && this->funct3 != 3) ||
        (op == Opcode::LOAD && this->funct3 != 4 && this->funct3 != 5))
        this->imm = (this->imm << 20) >> 20;
}
const std::string& IInstruction::toString() {
    if (this->printOut != "") return this->printOut;
    return this->printOut;
}  // TODO: implement

SInstruction::SInstruction(Opcode op, uint32_t raw) : op(op) {
    this->funct3 = (raw >> 12) & 0x07;
    this->rs1 = (raw >> 15) & 0x1F;
    this->rs2 = (raw >> 20) & 0x1F;
    this->imm = ((raw >> 7) & 0x17) | ((raw >> 25) << 5);

    this->imm = (this->imm << 20) >> 20;
}
const std::string& SInstruction::toString() {
    if (this->printOut != "") return this->printOut;
    return this->printOut;
}  // TODO: implement

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
    return this->printOut;
}  // TODO: implement

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
}  // TODO: implement

}  // namespace Disassembler