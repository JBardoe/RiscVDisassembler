#ifndef ARMINSTRUCTION_HPP
#define ARMINSTRUCTION_HPP

#include <string>

#include "translator/ArmTypes.hpp"

namespace Translator {

/**
 * Virtual class for an ARM assembly instruction
 */
class ArmInstruction {
   public:
    ArmInstruction(Operator instr) : instr(instr), printOut("") {}
    virtual ~ArmInstruction() = default;

    /**
     * Default toString method to print the instruction
     * Overridden for specific instructions
     *
     * @return the empty string
     */
    virtual const std::string& toString() { return this->printOut; };

    Operator instr;        // Instruction
    std::string printOut;  // String version of the instruction
};

/**
 * Represents an instruction that uses three registers
 *
 * Includes: add, sub, eor, orr, and, lsl, lsr, asr
 */
class RRRInstruction : public virtual ArmInstruction {
   public:
    RRRInstruction(Operator op, Register wd, Register wn, Register wm)
        : ArmInstruction(op), wd(wd), wn(wn), wm(wm) {}

    /**
     * toString method to print the instruction in assembly form
     *
     * @return string readout version of the instruction
     */
    const std::string& toString() override;

    // The three registers in the order they are listed in the instruction
    Register wd;
    Register wn;
    Register wm;
};

/**
 * Represents an instruction that uses two registers
 *
 * Includes: cmp, cset
 */
class RRInstruction : public virtual ArmInstruction {
   public:
    RRInstruction(Operator op, Register wd, Register wn)
        : ArmInstruction(op), wd(wd), wn(wn) {}

    /**
     * toString method to print the instruction in assembly form
     *
     * @return string readout version of the instruction
     */
    const std::string& toString() override;

    // The two registers in the order they are listed in the instruction
    Register wd;
    Register wn;
};

/**
 * Represents an instruction that uses two registers and an immediate
 *
 * Includes: add, eor, orr, and, lsl, lsr, asr, ldrsb, ldrsh, lsrsw, ldrb, ldrh,
 * strb, strh, str
 */
class RRIInstruction : public virtual ArmInstruction {
   public:
    RRIInstruction(Operator op, Register wd, Register wn, int imm, bool shift)
        : ArmInstruction(op), wd(wd), wn(wn), imm(imm), shift(shift) {}

    /**
     * toString method to print the instruction in assembly form
     *
     * @return string readout version of the instruction
     */
    const std::string& toString() override;

    // The two registers and immediate in the order of the instruction
    Register wd;
    Register wn;
    int imm;

    // Whether the instruction features a left shift at the end
    bool shift;
};

/**
 * Represents an instruction that uses one instruction and an immediate
 *
 * Includes: cmp, adr, movz
 */
class RIInstruction : public virtual ArmInstruction {
   public:
    RIInstruction(Operator op, Register wd, int imm)
        : ArmInstruction(op), wd(wd), imm(imm) {}

    /**
     * toString method to print the instruction in assembly form
     *
     * @return string readout version of the instruction
     */
    const std::string& toString() override;

    // The register and immediate in the order they are listed in the
    // instruction
    Register wd;
    int imm;
};

/**
 * Represents a branch instruction with an immediate
 *
 * Includes: beq, bne, blt, bge, blo, bhs, bl, b
 */
class BIInstruction : public virtual ArmInstruction {
   public:
    BIInstruction(Operator op, int imm) : ArmInstruction(op), imm(imm) {}

    /**
     * toString method to print the instruction in assembly form
     *
     * @return string readout version of the instruction
     */
    const std::string& toString() override;

    // Branch immediate
    int imm;
};

/**
 * Represents a branch instruction with an entry point
 *
 * Includes: beq, bne, blt, bge, blo, bhs, bl, b
 */
class BLInstruction : public virtual ArmInstruction {
   public:
    BLInstruction(Operator op, std::string label)
        : ArmInstruction(op), label(label) {}

    /**
     * toString method to print the instruction in assembly form
     *
     * @return string readout version of the instruction
     */
    const std::string& toString() override;

    // Label to jump to
    std::string label;
};

/**
 * Represents a branch instruction to a register value
 *
 * Includes: blr, br
 */
class BRInstruction : public virtual ArmInstruction {
   public:
    BRInstruction(Operator op, Register wd) : ArmInstruction(op), wd(wd) {}

    /**
     * toString method to print the instruction in assembly form
     *
     * @return string readout version of the instruction
     */
    const std::string& toString() override;

    // The register whose value to jump to
    Register wd;
};

/**
 * Represents an environment instruction
 *
 * Includes: svc, brk
 */
class EInstruction : public virtual ArmInstruction {
   public:
    EInstruction(Operator op) : ArmInstruction(op) {}

    /**
     * toString method to print the instruction in assembly form
     *
     * @return string readout version of the instruction
     */
    const std::string& toString() override;
};

/**
 * Represents an instruction that uses a register and symbol
 *
 * Includes: adr, ldrsb, ldrsh, ldrsw, strb, strh, str
 */
class RSInstruction : public virtual ArmInstruction {
   public:
    RSInstruction(Operator op, Register wd, std::string symbol)
        : ArmInstruction(op), wd(wd), symbol(symbol) {}

    /**
     * toString method to print the instruction in assembly form
     *
     * @return string readout version of the instruction
     */
    const std::string& toString() override;

    // The register and symbol
    Register wd;
    std::string symbol;
};

/**
 * Represents an entry point
 */
class EntryPoint : public virtual ArmInstruction {
   public:
    EntryPoint(std::string name)
        : ArmInstruction(Operator::entry), name(name) {}

    /**
     * toString method to print the instruction in assembly form
     *
     * @return string readout version of the instruction
     */
    const std::string& toString() override;

    // Name of the entry point
    std::string name;
};

}  // namespace Translator

#endif