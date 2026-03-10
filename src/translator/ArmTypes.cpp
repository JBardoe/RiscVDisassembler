#include "translator/ArmTypes.hpp"

namespace Translator {

std::string to_string(Register r) {
    switch (r) {
        case Register::wzr:
            return "wzr";
        case Register::lr:
            return "lr";
        case Register::sp:
            return "sp";
        case Register::xxgp:
            return "xxgp";
        case Register::xxtp:
            return "xxtp";
        case Register::w9:
            return "w9";
        case Register::w10:
            return "w10";
        case Register::w11:
            return "w11";
        case Register::fp:
            return "fp";
        case Register::w19:
            return "w19";
        case Register::w0:
            return "w0";
        case Register::w1:
            return "w1";
        case Register::w2:
            return "w2";
        case Register::w3:
            return "w3";
        case Register::w4:
            return "w4";
        case Register::w5:
            return "w5";
        case Register::w6:
            return "w6";
        case Register::w8:
            return "w8";
        case Register::w20:
            return "w20";
        case Register::w21:
            return "w21";
        case Register::w22:
            return "w22";
        case Register::w23:
            return "w23";
        case Register::w24:
            return "w24";
        case Register::w25:
            return "w25";
        case Register::w26:
            return "w26";
        case Register::w27:
            return "w27";
        case Register::w28:
            return "w28";
        case Register::w7:
            return "w7";
        case Register::w12:
            return "w12";
        case Register::w13:
            return "w13";
        case Register::w14:
            return "w14";
        case Register::w15:
            return "w15";
        case Register::lt:
            return "lt";
        case Register::lo:
            return "lo";
        default:
            return "<unknown_register>";
    }
}

std::string to_string(Operator i) {
    switch (i) {
        case Operator::add:
            return "add";
        case Operator::sub:
            return "sub";
        case Operator::eor:
            return "eor";
        case Operator::orr:
            return "orr";
        case Operator::And:
            return "and";
        case Operator::lsl:
            return "lsl";
        case Operator::lsr:
            return "lsr";
        case Operator::asr:
            return "asr";
        case Operator::cmp:
            return "cmp";
        case Operator::cset:
            return "cset";
        case Operator::ldrsb:
            return "ldrsb";
        case Operator::ldrsh:
            return "ldrsh";
        case Operator::ldr:
            return "ldr";
        case Operator::ldrb:
            return "ldrb";
        case Operator::ldrh:
            return "ldrh";
        case Operator::strb:
            return "strb";
        case Operator::strh:
            return "strh";
        case Operator::str:
            return "str";
        case Operator::beq:
            return "b.eq";
        case Operator::bne:
            return "b.ne";
        case Operator::blt:
            return "b.lt";
        case Operator::bge:
            return "b.ge";
        case Operator::blo:
            return "b.lo";
        case Operator::bhs:
            return "b.hs";
        case Operator::bl:
            return "bl";
        case Operator::b:
            return "b";
        case Operator::adr:
            return "adr";
        case Operator::blr:
            return "blr";
        case Operator::br:
            return "br";
        case Operator::movz:
            return "movz";
        case Operator::svc:
            return "svc";
        case Operator::brk:
            return "brk";
        case Operator::entry:
            return "Entry Point: ";
        default:
            return "<unknown_operator>";
    }
}
}  // namespace Translator