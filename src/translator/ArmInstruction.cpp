#include "translator/ArmInstruction.hpp"

#include "utils/AssemblyTypes.hpp"

namespace Translator {

const std::string& RRRInstruction::toString() {
    if (printOut != "") return printOut;

    printOut = "\t" + to_string(instr) + " " + to_string(wd) + ", " +
               to_string(wn) + ", " + to_string(wm);

    if (this->printOut.size() < Assembly::COMMENT_COL)
        this->printOut.append(Assembly::COMMENT_COL - this->printOut.size(),
                              ' ');

    this->printOut += "// " + to_string(wd) + " = " + to_string(wn) + " ";

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
            break;
    }

    printOut += to_string(wm);

    return printOut;
}

const Analyser::InstructionAnalysis& RRRInstruction::getAnalysis() {
    return Analyser::InstructionAnalysis({wn, wm}, wd,
                                         Analyser::InstructionClass::ALU, -1);
}

const std::string& RRInstruction::toString() {
    if (printOut != "") return printOut;

    printOut =
        "\t" + to_string(instr) + " " + to_string(wd) + ", " + to_string(wn);

    if (this->printOut.size() < Assembly::COMMENT_COL)
        this->printOut.append(Assembly::COMMENT_COL - this->printOut.size(),
                              ' ');

    this->printOut += "// ";

    if (instr == Operator::cmp) {
        printOut += "set flags for " + to_string(wd) + " - " + to_string(wn);
    } else {
        printOut += to_string(wd) + " = most_recent_cmp_less_than?1:0 ";
        if (wn == Register::lo) printOut += "(unsigned)";
    }

    return printOut;
}

const Analyser::InstructionAnalysis& RRInstruction::getAnalysis() {
    if (instr == Operator::cmp) {
        return Analyser::InstructionAnalysis(
            {wd, wn}, Register::empty, Analyser::InstructionClass::ALU, -1);
    }
    return Analyser::InstructionAnalysis({wn}, wd,
                                         Analyser::InstructionClass::ALU, -1);
}

const std::string& RRIInstruction::toString() {
    if (printOut != "") return printOut;

    printOut = "\t" + to_string(instr) + " " + to_string(wd) + ", ";

    std::string suffix = "";

    switch (instr) {
        case Operator::ldrsb:
            suffix = "[0:7] (sign-extends)";
            goto loads;
        case Operator::ldrsh:
            suffix = "[0:15] (sign-extends)";
            goto loads;
        case Operator::ldr:
            suffix = "[0:31]";
            goto loads;
        case Operator::ldrb:
            suffix = "[0:7] (zero-entends)";
            goto loads;
        case Operator::ldrh:
            suffix = "[0:15] (zero-extends)";
        loads:
            printOut += "[" + to_string(wn) + ", #" + std::to_string(imm) + "]";

            if (this->printOut.size() < Assembly::COMMENT_COL)
                this->printOut.append(
                    Assembly::COMMENT_COL - this->printOut.size(), ' ');

            this->printOut += "// " + to_string(wd) + " = Memory[" +
                              to_string(wn) + "+" + std::to_string(imm) + "]" +
                              suffix;
            break;

        case Operator::strb:
            suffix = "[0:7]";
            goto stores;
        case Operator::strh:
            suffix = "[0:15]";
            goto stores;
        case Operator::str:
            suffix = "[0:31]";
        stores:
            printOut += "[" + to_string(wn) + ", #" + std::to_string(imm) + "]";

            if (this->printOut.size() < Assembly::COMMENT_COL)
                this->printOut.append(
                    Assembly::COMMENT_COL - this->printOut.size(), ' ');

            this->printOut += "// Memory[" + to_string(wd) + "+" +
                              std::to_string(imm) + "]" + suffix + " = " +
                              to_string(wn) + suffix;
            break;

        case Operator::add:
            suffix = " + ";
            goto ops;
        case Operator::eor:
            suffix = " ^ ";
            goto ops;
        case Operator::orr:
            suffix = " | ";
            goto ops;
        case Operator::And:
            suffix = " & ";
            goto ops;
        case Operator::lsl:
            suffix = " << ";
            goto ops;
        case Operator::lsr:
            suffix = " >> ";
            goto ops;
        case Operator::asr:
            suffix = " >> ";
        ops:
            printOut += to_string(wn) + ", #" + std::to_string(imm);
            if (shift) printOut += ", lsl #12";

            if (this->printOut.size() < Assembly::COMMENT_COL)
                this->printOut.append(
                    Assembly::COMMENT_COL - this->printOut.size(), ' ');

            this->printOut += "// " + to_string(wd) + " = " + to_string(wn) +
                              suffix + std::to_string(imm);

            break;
        default:
            break;
    }

    return printOut;
}

const Analyser::InstructionAnalysis&
RRIInstruction::getAnalysis() {  // TODO implement
}

const std::string& RIInstruction::toString() {
    if (printOut != "") return printOut;

    printOut = "\t" + to_string(instr) + " " + to_string(wd) + ", ";

    if (instr == Operator::adr) {
        printOut += ".+" + std::to_string(imm);

        if (this->printOut.size() < Assembly::COMMENT_COL)
            this->printOut.append(Assembly::COMMENT_COL - this->printOut.size(),
                                  ' ');

        this->printOut +=
            "// " + to_string(wd) + " = PC + " + std::to_string(imm);
    } else if (instr == Operator::cmp) {
        printOut += "#" + std::to_string(imm) + "set flags for " +
                    to_string(wd) + " - " + std::to_string(imm);
    } else {
        printOut += "#" + std::to_string(imm);

        if (this->printOut.size() < Assembly::COMMENT_COL)
            this->printOut.append(Assembly::COMMENT_COL - this->printOut.size(),
                                  ' ');

        this->printOut += "// " + to_string(wd) + " = " + std::to_string(imm) +
                          " (zero-extended)";
    }

    return printOut;
}

const Analyser::InstructionAnalysis&
RIInstruction::getAnalysis() {  // TODO implement
}

const std::string& BIInstruction::toString() {
    if (printOut != "") return printOut;

    printOut = "\t" + to_string(instr) + " " + std::to_string(imm);

    if (this->printOut.size() < Assembly::COMMENT_COL)
        this->printOut.append(Assembly::COMMENT_COL - this->printOut.size(),
                              ' ');

    this->printOut += "// PC+=" + std::to_string(imm) + " ";

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
            break;
    }

    return printOut;
}

const Analyser::InstructionAnalysis& BIInstruction::getAnalysis() {
    if (instr == Operator::b || instr == Operator::bl) {
        return Analyser::InstructionAnalysis(
            {}, Register::empty, Analyser::InstructionClass::JUMP, imm);
    }

    return Analyser::InstructionAnalysis(
        {}, Register::empty, Analyser::InstructionClass::BRANCH, imm);
}

const std::string& BLInstruction::toString() {
    if (printOut != "") return printOut;

    printOut = "\t" + to_string(instr) + " " + label;

    if (this->printOut.size() < Assembly::COMMENT_COL)
        this->printOut.append(Assembly::COMMENT_COL - this->printOut.size(),
                              ' ');

    this->printOut += "// Branch to " + label + " ";

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
            break;
    }

    return printOut;
}

const Analyser::InstructionAnalysis& BLInstruction::getAnalysis() {
    if (instr == Operator::b || instr == Operator::bl) {
        return Analyser::InstructionAnalysis(
            {}, Register::empty, Analyser::InstructionClass::JUMP, -1);
    }

    return Analyser::InstructionAnalysis(
        {}, Register::empty, Analyser::InstructionClass::BRANCH, -1);
}

const std::string& BRInstruction::toString() {
    if (printOut != "") return printOut;

    printOut = "\t" + to_string(instr) + " " + to_string(wd);

    if (this->printOut.size() < Assembly::COMMENT_COL)
        this->printOut.append(Assembly::COMMENT_COL - this->printOut.size(),
                              ' ');

    this->printOut += "// PC+=" + to_string(wd) + " ";

    if (instr == Operator::blr) {
        printOut += "and set lr to PC+4";
    }

    return printOut;
}

const Analyser::InstructionAnalysis& BRInstruction::getAnalysis() {
    return Analyser::InstructionAnalysis({wd}, Register::empty,
                                         Analyser::InstructionClass::JUMP, -1);
}

const std::string& EInstruction::toString() {
    if (printOut != "") return printOut;
    printOut = "\t" + to_string(instr) + " #0";

    if (this->printOut.size() < Assembly::COMMENT_COL)
        this->printOut.append(Assembly::COMMENT_COL - this->printOut.size(),
                              ' ');

    this->printOut += "//";

    if (instr == Operator::svc) {
        printOut += " Transfer control to OS";
    } else {
        printOut += " Transfer control to debugger";
    }

    return printOut;
}

const Analyser::InstructionAnalysis&
EInstruction::getAnalysis() {  // TODO implement
}

const std::string& RSInstruction::toString() {
    if (printOut != "") return printOut;

    printOut = "\t" + to_string(instr) + ", " + to_string(wd) + ", ";

    if (instr == Operator::ldrsb || instr == Operator::ldrsh ||
        instr == Operator::ldr) {
        printOut += "[" + symbol + "]";
    } else {
        printOut += symbol;
    }
    if (this->printOut.size() < Assembly::COMMENT_COL)
        this->printOut.append(Assembly::COMMENT_COL - this->printOut.size(),
                              ' ');

    this->printOut += "// ";

    std::string suffix = "";

    switch (instr) {
        case Operator::ldrsb:
            suffix = "[0:7]";
            goto loads;
        case Operator::ldrsh:
            suffix = "[0:15]";
            goto loads;
        case Operator::ldr:
            suffix = "[0:31]";
        loads:
            printOut += to_string(wd) + " = " + symbol + suffix;
            break;
        case Operator::strb:
            suffix = "[0:7]";
            goto stores;
        case Operator::strh:
            suffix = "[0:15]";
            goto stores;
        case Operator::str:
            suffix = "[0:31]";
        stores:
            printOut += symbol + suffix + " = " + to_string(wd);
            break;
        default:
            printOut += to_string(wd) + " = &" + symbol;
    }

    return printOut;
}

const Analyser::InstructionAnalysis&
RSInstruction::getAnalysis() {  // TODO implement
}

const std::string& EntryPoint::toString() {
    if (printOut != "") return printOut;

    printOut = "\n" + name + ":";

    return printOut;
}
}  // namespace Translator