#include "translator/ArmInstruction.hpp"
namespace Translator {

const std::string& RRRInstruction::toString() {
    if (printOut != "") return printOut;

    printOut = "\t" + to_string(instr) + " " + to_string(wd) + ", " +
               to_string(wn) + ", " + to_string(wm) + "\t// " + to_string(wd) +
               " = " + to_string(wn) + " ";

    switch (instr) {
        case Operator::add:
            printOut += "+ ";
            break;
        case Operator::sub:
            printOut += "- ";
            break;
        case Operator::eor:
            printOut += "^ ";
            break;
        case Operator::orr:
            printOut += "| ";
            break;
        case Operator::And:
            printOut += "& ";
            break;
        case Operator::lsl:
            printOut += "<< ";
            break;
        case Operator::lsr:
            printOut += ">> ";
            break;
        case Operator::asr:
            printOut += ">> ";
            break;
        default:
    }

    printOut += to_string(wm);
}

const std::string& RRInstruction::toString() {
    if (printOut != "") return printOut;

    printOut = "\t" + to_string(instr) + " " + to_string(wd) + ", " +
               to_string(wn) + "\t// ";

    if (instr == Operator::cmp) {
        printOut += "set flags for " + to_string(wd) + " - " + to_string(wn);
    } else {
        printOut += to_string(wd) + " = most_recent_cmp_less_than?1:0 ";
        if (wn == Register::lo) printOut += "(unsigned)";
    }

    return printOut;
}

const std::string& RRIInstruction::toString() {  // TODO implement
    if (printOut != "") return printOut;
}

const std::string& RIInstruction::toString() {  // TODO implement
    if (printOut != "") return printOut;
}

const std::string& BIInstruction::toString() {  // TODO implement
    if (printOut != "") return printOut;
}

const std::string& BLInstruction::toString() {  // TODO implement
    if (printOut != "") return printOut;

    printOut =
        "\t" + to_string(instr) + " " + label + "\t// branch to " + label + " ";

    switch (instr) {
        case Operator::beq:
            printOut += "if most_recent_cmp was equal";
            break;
        case Operator::bne:
            printOut += "if most_recent_cmp was not equal";
            break;
        case Operator::blt:
            printOut += "if most_recent_cmp was less than";
            break;
        case Operator::bge:
            printOut += "if most_recent_cmp was greater than or equal to";
            break;
        case Operator::blo:
            printOut += "if most_recent_cmp was less than (unsigned)";
            break;
        case Operator::bhs:
            printOut +=
                "if most_recent_cmp was greater than or equal to (unsigned)";
            break;
        case Operator::bl:
            printOut += "and set lr to PC+4";
            break;
        default:
    }

    return printOut;
}

const std::string& EInstruction::toString() {
    if (printOut != "") return printOut;
    printOut = "\t" + to_string(instr) + " #0\t//";

    if (instr == Operator::svc) {
        printOut += " Transfer control to OS";
    } else {
        printOut += " Transfer control to debugger";
    }

    return printOut;
}

const std::string& EntryPoint::toString() {
    if (printOut != "") return printOut;

    printOut = "\n" + name + ":";

    return printOut;
}
}  // namespace Translator