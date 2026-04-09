#ifndef ARMTYPES_HPP
#define ARMTYPES_HPP

#include <string>

namespace Translator {

/**
 * Enum representing an AAarch64 register (matching order to the RISC-V register
 * enum)
 */
enum class Register : int {
    xzr,
    lr,
    sp,
    xxgp,  // Eliminated
    xxtp,  // Eliminated
    x9,
    x10,
    x11,
    fp,
    x19,
    x0,
    x1,
    x2,
    x3,
    x4,
    x5,
    x6,
    x8,
    x20,
    x21,
    x22,
    x23,
    x24,
    x25,
    x26,
    x27,
    x28,
    x7,
    x12,
    x13,
    x14,
    x15,
    lt,
    lo,
    empty,  // Stand-in register
};

/**
 * toString for the register enum
 *
 * @param r register enum
 * @return string equivalent
 */
std::string to_string(Register r);

/**
 * All relevant ARM instructions
 */
enum class Operator : int {
    add,
    sub,
    eor,
    orr,
    And,
    lsl,
    lsr,
    asr,
    cmp,
    cset,
    ldrsw,
    ldrsb,
    ldrsh,
    ldrb,
    ldrh,
    strb,
    strh,
    str,
    beq,
    bne,
    blt,
    bge,
    blo,
    bhs,
    bl,
    b,
    adr,
    blr,
    br,
    movz,
    svc,
    brk,
    entry,  // Fake instruction denoting an entry point
};

/**
 * toString for the instruction enum
 *
 * @param i instruction enum
 * @return string equivalent
 */
std::string to_string(Operator i);

}  // namespace Translator

#endif