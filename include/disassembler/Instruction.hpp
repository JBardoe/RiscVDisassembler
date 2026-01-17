#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <cstdint>
#include <string>

#include "disassembler/RiscvTypes.hpp"

namespace Disassembler {

/**
 * Virtual class for an assembly instruction
 */
class Instruction {
   public:
    Instruction() : printOut("") {};
    virtual ~Instruction() = default;

    /**
     * Default toString method to print the instruction
     * Overridden for specific instructions
     *
     * @return the empty string
     */
    virtual const std::string& toString() { return this->printOut; };

   protected:
    std::string printOut;  // String version of the instruction if it has
                           // already been generated
};

/**
 * RISC-V R-Type (Register-Register) instruction
 */
class RInstruction : public virtual Instruction {
   public:
    RInstruction(RISCV::Opcode op, uint32_t raw);

    /**
     * toString method to print the instruction in assembly form
     *
     * @return string readout version of the instruction
     */
    const std::string& toString() override;

   private:
    RISCV::Opcode op;  // Opcode

    /**
     * R-Type instructions have the form:
     *
     * instr rd, rs1, rs2
     */
    RISCV::Instruction instr;
    RISCV::Register rd;
    RISCV::Register rs1;
    RISCV::Register rs2;
};

/**
 * RISC-V I-Type (Immediate) instruction
 */
class IInstruction : public virtual Instruction {
   public:
    IInstruction(RISCV::Opcode op, uint32_t raw);

    /**
     * toString method to print the instruction in assembly form
     *
     * @return string readout version of the instruction
     */
    const std::string& toString() override;

   private:
    RISCV::Opcode op;  // Opcode

    /**
     * I-Type instructions have the form:
     *
     * instr rd, rs1, imm
     * OR
     * intr rd, imm(rs1)
     */
    RISCV::Instruction instr;
    RISCV::Register rd;
    RISCV::Register rs1;
    int imm;
};

/**
 * RISC-V S-Type (Store) instruction
 */
class SInstruction : public virtual Instruction {
   public:
    SInstruction(RISCV::Opcode op, uint32_t raw);

    /**
     * toString method to print the instruction in assembly form
     *
     * @return string readout version of the instruction
     */
    const std::string& toString() override;

   private:
    RISCV::Opcode op;  // Opcode

    /**
     * S-Type instructions have the form:
     *
     * instr rs2, imm(rs1)
     */
    RISCV::Instruction instr;
    RISCV::Register rs1;
    RISCV::Register rs2;
    int imm;
};

/**
 * RISC-V B-Type (Branch) instruction
 */
class BInstruction : public virtual Instruction {
   public:
    BInstruction(RISCV::Opcode op, uint32_t raw);

    /**
     * toString method to print the instruction in assembly form
     *
     * @return string readout version of the instruction
     */
    const std::string& toString() override;

   private:
    RISCV::Opcode op;  // Opcode

    /**
     * B-Type instructions have the form:
     *
     * instr rs1, rs2, imm
     */
    RISCV::Instruction instr;
    RISCV::Register rs1;
    RISCV::Register rs2;
    int imm;
};

/**
 * RISC-V U-Type (Upper-Immediate) instruction
 */
class UInstruction : public virtual Instruction {
   public:
    UInstruction(RISCV::Opcode op, uint32_t raw);

    /**
     * toString method to print the instruction in assembly form
     *
     * @return string readout version of the instruction
     */
    const std::string& toString() override;

   private:
    RISCV::Opcode op;  // Opcode

    /**
     * U-Type instructions have the form:
     *
     * instr rd, imm
     */
    RISCV::Instruction instr;
    RISCV::Register rd;
    int imm;
};

/**
 * RISC-V J-Type (Jump) instruction
 */
class JInstruction : public virtual Instruction {
   public:
    JInstruction(RISCV::Opcode op, uint32_t raw);

    /**
     * toString method to print the instruction in assembly form
     *
     * @return string readout version of the instruction
     */
    const std::string& toString() override;

   private:
    RISCV::Opcode op;  // Opcode

    /**
     * J-Type instructions have the form:
     *
     * instr rd, imm
     */
    RISCV::Instruction instr;
    RISCV::Register rd;
    int imm;
};

}  // namespace Disassembler

#endif