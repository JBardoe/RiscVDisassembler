#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <cstdint>
#include <string>

#include "disassembler/DisassemblerTypes.hpp"

namespace Disassembler {

class Instruction {
   public:
    Instruction() = default;
    virtual ~Instruction() = default;
    virtual const std::string& toString();
};

class RInstruction : public virtual Instruction {
   public:
    RInstruction(Opcode op, uint32_t raw);
    const std::string& toString() override;

   private:
    Opcode op;
    int rd;
    int funct3;
    int rs1;
    int rs2;
    int funct7;

    std::string printOut;
};

class IInstruction : public virtual Instruction {
   public:
    IInstruction(Opcode op, uint32_t raw);
    const std::string& toString() override;

   private:
    Opcode op;
    int rd;
    int funct3;
    int rs1;
    int imm;

    std::string printOut;
};

class SInstruction : public virtual Instruction {
   public:
    SInstruction(Opcode op, uint32_t raw);
    const std::string& toString() override;

   private:
    Opcode op;
    int imm;
    int funct3;
    int rs1;
    int rs2;

    std::string printOut;
};

class BInstruction : public virtual Instruction {
   public:
    BInstruction(Opcode op, uint32_t raw);
    const std::string& toString() override;

   private:
    Opcode op;
    int imm;
    int funct3;
    int rs1;
    int rs2;

    std::string printOut;
};

class UInstruction : public virtual Instruction {
   public:
    UInstruction(Opcode op, uint32_t raw);
    const std::string& toString() override;

   private:
    Opcode op;
    int rd;
    int imm;

    std::string printOut;
};

class JInstruction : public virtual Instruction {
   public:
    JInstruction(Opcode op, uint32_t raw);
    const std::string& toString() override;

   private:
    Opcode op;
    int rd;
    int imm;

    std::string printOut;
};

}  // namespace Disassembler

#endif