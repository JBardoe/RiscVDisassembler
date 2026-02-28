#include "translator/ArmInstruction.hpp"
namespace Translator {

const std::string& RRRInstruction::toString() {  // TODO implement
    if (printOut != "") return printOut;
}

const std::string& RRInstruction::toString() {  // TODO implement
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