#ifndef RISCVINSTRUCTION_H
#define RISCVINSTRUCTION_H

#include <cstdint>
#include <string>

#include "disassembler/RiscvTypes.hpp"

namespace Disassembler {

/**
 * Virtual class for an assembly instruction
 */
class RiscvInstruction {
   public:
    RiscvInstruction() : printOut("") {};
    RiscvInstruction(Operator instr) : instr(instr) {};
    RiscvInstruction(Opcode op) : op(op) {}
    virtual ~RiscvInstruction() = default;

    /**
     * Default toString method to print the instruction
     * Overridden for specific instructions
     *
     * @return the empty string
     */
    virtual const std::string& toString() { return this->printOut; };

    Operator instr;
    Opcode op;
    std::string printOut;  // String version of the instruction if it has
                           // already been generated
};

/**
 * RISC-V R-Type (Register-Register) instruction
 */
class RInstruction : public virtual RiscvInstruction {
   public:
    RInstruction(Opcode op, uint32_t raw);

    /**
     * toString method to print the instruction in assembly form
     *
     * @return string readout version of the instruction
     */
    const std::string& toString() override;

    /**
     * R-Type instructions have the form:
     *
     * instr rd, rs1, rs2
     */
    Register rd;
    Register rs1;
    Register rs2;
};

/**
 * RISC-V I-Type (Immediate) instruction
 */
class IInstruction : public virtual RiscvInstruction {
   public:
    IInstruction(Opcode op, uint32_t raw);

    /**
     * toString method to print the instruction in assembly form
     *
     * @return string readout version of the instruction
     */
    const std::string& toString() override;

    /**
     * I-Type instructions have the form:
     *
     * instr rd, rs1, imm
     * OR
     * intr rd, imm(rs1)
     */
    Register rd;
    Register rs1;
    int imm;
};

/**
 * RISC-V S-Type (Store) instruction
 */
class SInstruction : public virtual RiscvInstruction {
   public:
    SInstruction(Opcode op, uint32_t raw);

    /**
     * toString method to print the instruction in assembly form
     *
     * @return string readout version of the instruction
     */
    const std::string& toString() override;

    /**
     * S-Type instructions have the form:
     *
     * instr rs2, imm(rs1)
     */
    Register rs1;
    Register rs2;
    int imm;
};

/**
 * RISC-V B-Type (Branch) instruction
 */
class BInstruction : public virtual RiscvInstruction {
   public:
    BInstruction(Opcode op, uint32_t raw);

    /**
     * toString method to print the instruction in assembly form
     *
     * @return string readout version of the instruction
     */
    const std::string& toString() override;

    /**
     * B-Type instructions have the form:
     *
     * instr rs1, rs2, imm
     */
    Register rs1;
    Register rs2;
    int imm;
};

/**
 * RISC-V U-Type (Upper-Immediate) instruction
 */
class UInstruction : public virtual RiscvInstruction {
   public:
    UInstruction(Opcode op, uint32_t raw, uint32_t addr);

    /**
     * toString method to print the instruction in assembly form
     *
     * @return string readout version of the instruction
     */
    const std::string& toString() override;

    /**
     * U-Type instructions have the form:
     *
     * instr rd, imm
     */
    Register rd;
    int imm;
    uint32_t addr;
};

/**
 * RISC-V J-Type (Jump) instruction
 */
class JInstruction : public virtual RiscvInstruction {
   public:
    JInstruction(Opcode op, uint32_t raw);

    /**
     * toString method to print the instruction in assembly form
     *
     * @return string readout version of the instruction
     */
    const std::string& toString() override;

    /**
     * J-Type instructions have the form:
     *
     * instr rd, imm
     */
    Register rd;
    int imm;
};

class PseudoLoadInstruction : public virtual RiscvInstruction {
   public:
    PseudoLoadInstruction(Operator instr, Register rd, std::string symbol)
        : RiscvInstruction(instr), rd(rd), symbol(symbol) {};

    const std::string& toString() override;

    Register rd;
    std::string symbol;
};

class PseudoStoreInstruction : public virtual RiscvInstruction {
   public:
    PseudoStoreInstruction(Operator instr, Register rd, std::string symbol,
                           Register rt)
        : RiscvInstruction(instr), rd(rd), symbol(symbol), rt(rt) {}

    const std::string& toString() override;

    Register rd;
    std::string symbol;
    Register rt;
};

class EntryPoint : public virtual RiscvInstruction {
   public:
    EntryPoint(std::string name)
        : RiscvInstruction(Operator::entry), name(name) {};

    const std::string& toString() override;

    std::string name;
};

class BInstructionEntry : public virtual RiscvInstruction {
   public:
    BInstructionEntry(BInstruction* old, std::string entryPoint)
        : RiscvInstruction(old->instr),
          rs1(old->rs1),
          rs2(old->rs2),
          entryPoint(entryPoint) {}

    const std::string& toString() override;

    Register rs1;
    Register rs2;
    std::string entryPoint;
};

class JInstructionEntry : public virtual RiscvInstruction {
   public:
    JInstructionEntry(JInstruction* old, std::string entryPoint)
        : RiscvInstruction(old->instr), rd(old->rd), entryPoint(entryPoint) {}

    const std::string& toString() override;

    Register rd;
    std::string entryPoint;
};

}  // namespace Disassembler

#endif