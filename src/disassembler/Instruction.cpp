#include "disassembler/Instruction.hpp"

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

}  // TODO: implement

IInstruction::IInstruction(Opcode op, uint32_t raw) : op(op) {
    this->rd = (raw >> 7) & 0x1F;
    this->funct3 = (raw >> 12) & 0x07;
    this->rs1 = (raw >> 15) & 0x1F;
    this->imm = (raw >> 20);

    this->imm = (this->imm << 20) >> 20;
}
const std::string& IInstruction::toString() {
    if (this->printOut != "") return this->printOut;

}  // TODO: implement

SInstruction::SInstruction(Opcode op, uint32_t raw) {
    this->funct3 = (raw >> 12) & 0x07;
    this->rs1 = (raw >> 15) & 0x1F;
    this->rs2 = (raw >> 20) & 0x1F;
    this->imm = ((raw >> 7) & 0x17) | ((raw >> 25) << 5);

    this->imm = (this->imm << 20) >> 20;
}
const std::string& SInstruction::toString() {
    if (this->printOut != "") return this->printOut;

}  // TODO: implement

BInstruction::BInstruction(Opcode op, uint32_t raw) {
    this->funct3 = (raw >> 12) & 0x07;
    this->rs1 = (raw >> 15) & 0x1F;
    this->rs2 = (raw >> 20) & 0x1F;
    this->imm = (((raw >> 31) & 0x01) << 12) | (((raw >> 7) & 0x01) << 11) |
                (((raw >> 25) & 0x3F) << 5) | (((raw >> 8) & 0x0F) << 1);

    this->imm = (this->imm << 19) >> 19;
}
const std::string& BInstruction::toString() {
    if (this->printOut != "") return this->printOut;

}  // TODO: implement

UInstruction::UInstruction(Opcode op, uint32_t raw) {
    this->rd = (raw >> 7) & 0x1F;
    this->imm = (raw >> 12);
}
const std::string& UInstruction::toString() {
    if (this->printOut != "") return this->printOut;

}  // TODO: implement

JInstruction::JInstruction(Opcode op, uint32_t raw) {
    this->rd = (raw >> 7) & 0x1F;
    this->imm = imm =
        (((raw >> 31) & 0x1) << 20) | (((raw >> 12) & 0xFF) << 12) |
        (((raw >> 20) & 0x1) << 11) | (((raw >> 21) & 0x3FF) << 1);

    this->imm = (this->imm << 11) >> 11;
}
const std::string& JInstruction::toString() {
    if (this->printOut != "") return this->printOut;

}  // TODO: implement
}  // namespace Disassembler