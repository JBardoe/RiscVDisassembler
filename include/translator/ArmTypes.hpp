#ifndef AARCHTYPES_HPP
#define ARRCHTYPES_HPP

#include <string>

namespace Translator {
enum class Register : int {
    wzr,
    lr,
    sp,
    xxgp,  // Eliminated
    xxtp,  // Eliminated
    w9,
    w10,
    w11,
    fp,
    w19,
    w0,
    w1,
    w2,
    w3,
    w4,
    w5,
    w6,
    w8,
    w20,
    w21,
    w22,
    w23,
    w24,
    w25,
    w26,
    w27,
    w28,
    w7,
    w12,
    w13,
    w14,
    w15,
    lt,
    lo,
};

std::string to_string(Register r);

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
    ldrsb,
    ldrsh,
    ldrsw,
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
};

std::string to_string(Operator i);

}  // namespace Translator

#endif