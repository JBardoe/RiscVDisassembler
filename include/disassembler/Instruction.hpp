#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <cstdint>
#include <string>

#include "disassembler/RiscvTypes.hpp"

namespace Disassembler {

class Instruction {
   public:
    Instruction() : printOut("") {};
    virtual ~Instruction() = default;
    virtual const std::string& toString() { return this->printOut; };

   protected:
    std::string printOut;
};

class RInstruction : public virtual Instruction {
   public:
    RInstruction(RISCV::Opcode op, uint32_t raw);
    const std::string& toString() override;

   private:
    RISCV::Opcode op;

    RISCV::Instruction instr;
    RISCV::Register rd;
    RISCV::Register rs1;
    RISCV::Register rs2;
};

class IInstruction : public virtual Instruction {
   public:
    IInstruction(RISCV::Opcode op, uint32_t raw);
    const std::string& toString() override;

   private:
    RISCV::Opcode op;

    RISCV::Instruction instr;
    RISCV::Register rd;
    RISCV::Register rs1;
    int imm;
};

class SInstruction : public virtual Instruction {
   public:
    SInstruction(RISCV::Opcode op, uint32_t raw);
    const std::string& toString() override;

   private:
    RISCV::Opcode op;

    RISCV::Instruction instr;
    RISCV::Register rs1;
    RISCV::Register rs2;
    int imm;
};

class BInstruction : public virtual Instruction {
   public:
    BInstruction(RISCV::Opcode op, uint32_t raw);
    const std::string& toString() override;

   private:
    RISCV::Opcode op;

    RISCV::Instruction instr;
    RISCV::Register rs1;
    RISCV::Register rs2;
    int imm;
};

class UInstruction : public virtual Instruction {
   public:
    UInstruction(RISCV::Opcode op, uint32_t raw);
    const std::string& toString() override;

   private:
    RISCV::Opcode op;

    RISCV::Instruction instr;
    RISCV::Register rd;
    int imm;
};

class JInstruction : public virtual Instruction {
   public:
    JInstruction(RISCV::Opcode op, uint32_t raw);
    const std::string& toString() override;

   private:
    RISCV::Opcode op;

    RISCV::Instruction instr;
    RISCV::Register rd;
    int imm;
};

}  // namespace Disassembler

#endif