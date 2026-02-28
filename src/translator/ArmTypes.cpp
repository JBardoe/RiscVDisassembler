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
        default:
            return "<unknown_register>";
    }
}
}  // namespace Translator