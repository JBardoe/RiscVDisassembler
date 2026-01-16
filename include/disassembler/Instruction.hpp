#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <cstdint>
#include <string>

#include "disassembler/RiscvTypes.hpp"

namespace Disassembler {

class Instruction {
   public:
    Instruction() = default;
    virtual ~Instruction() = default;
    virtual const std::string& toString();
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
    std::string printOut;
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
    std::string printOut;
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
    std::string printOut;
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
    std::string printOut;
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
    std::string printOut;
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
    std::string printOut;
};

}  // namespace Disassembler

#endif