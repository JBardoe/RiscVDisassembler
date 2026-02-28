#ifndef ARMINSTRUCTION_HPP
#define ARMINSTRUCTION_HPP

#include <string>

#include "translator/ArmTypes.hpp"

namespace Translator {
class ArmInstruction {
   public:
    ArmInstruction(Operator instr) : instr(instr), printOut("") {}
    virtual ~ArmInstruction() = default;

    virtual const std::string& toString() { return this->printOut; };

    std::string printOut;
    Operator instr;
};

/*
add
sub
eor
orr
and
lsl
lsr
asr
*/
class RRRInstruction : public virtual ArmInstruction {
   public:
    RRRInstruction(Operator op, Register wd, Register wn, Register wm)
        : ArmInstruction(op), wd(wd), wn(wn), wm(wm) {}

    const std::string& toString() override;

    Register wd;
    Register wn;
    Register wm;
};

/*
cmp
cset
*/
class RRInstruction : public virtual ArmInstruction {
   public:
    RRInstruction(Operator op, Register wd, Register wn)
        : ArmInstruction(op), wd(wd), wn(wn) {}

    const std::string& toString() override;

    Register wd;
    Register wn;
};

/*
add
eor
orr
and
lsl
lsr
asr

ldrsb
ldrsh
lsrsw
ldrb
ldrh
strb
strh
str
*/
class RRIInstruction : public virtual ArmInstruction {
   public:
    RRIInstruction(Operator op, Register wd, Register wn, int imm)
        : ArmInstruction(op), wd(wd), wn(wn), imm(imm) {}

    const std::string& toString() override;

    Register wd;
    Register wn;
    int imm;
};

/*
cmp
adr
movz
*/
class RIInstruction : public virtual ArmInstruction {
   public:
    RIInstruction(Operator op, Register wd, int imm)
        : ArmInstruction(op), wd(wd), imm(imm) {}

    const std::string& toString() override;

    Register wd;
    int imm;
};

/*
beq
bne
blt
bge
blo
bhs

bl
b
blr
br
*/
class BIInstruction : public virtual ArmInstruction {
   public:
    BIInstruction(Operator op, int imm) : ArmInstruction(op), imm(imm) {}

    const std::string& toString() override;

    int imm;
};

/*
beq
bne
blt
bge
blo
bhs

bl
b
*/
class BLInstruction : public virtual ArmInstruction {
   public:
    BLInstruction(Operator op, std::string label)
        : ArmInstruction(op), label(label) {}

    const std::string& toString() override;

    std::string label;
};

/*
svc
brk
*/
class EInstruction : public virtual ArmInstruction {
   public:
    EInstruction(Operator op) : ArmInstruction(op) {}

    const std::string& toString() override;
};

class EntryPoint : public virtual ArmInstruction {
   public:
    EntryPoint(std::string name)
        : ArmInstruction(Operator::entry), name(name) {}

    const std::string& toString() override;

    std::string name;
};

// TODO blr + br

}  // namespace Translator

#endif