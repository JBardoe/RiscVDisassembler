#include "translator/ArmTypes.hpp"

namespace Translator {

std::string to_string(Register r) {
    switch (r) {
        case Register::xzr:
            return "xzr";
        case Register::lr:
            return "lr";
        case Register::sp:
            return "sp";
        case Register::xxgp:
            return "xxgp";
        case Register::xxtp:
            return "xxtp";
        case Register::x9:
            return "x9";
        case Register::x10:
            return "x10";
        case Register::x11:
            return "x11";
        case Register::fp:
            return "fp";
        case Register::x19:
            return "x19";
        case Register::x0:
            return "x0";
        case Register::x1:
            return "x1";
        case Register::x2:
            return "x2";
        case Register::x3:
            return "x3";
        case Register::x4:
            return "x4";
        case Register::x5:
            return "x5";
        case Register::x6:
            return "x6";
        case Register::x8:
            return "x8";
        case Register::x20:
            return "x20";
        case Register::x21:
            return "x21";
        case Register::x22:
            return "x22";
        case Register::x23:
            return "x23";
        case Register::x24:
            return "x24";
        case Register::x25:
            return "x25";
        case Register::x26:
            return "x26";
        case Register::x27:
            return "x27";
        case Register::x28:
            return "x28";
        case Register::x7:
            return "x7";
        case Register::x12:
            return "x12";
        case Register::x13:
            return "x13";
        case Register::x14:
            return "x14";
        case Register::x15:
            return "x15";
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
        case Operator::ldrsw:
            return "ldrsw";
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