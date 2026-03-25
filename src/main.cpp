#include <stdlib.h>

#include <cstdint>
#include <iomanip>
#include <iostream>
#include <string>

#include "analyser/Analyser.hpp"
#include "analyser/AnalysisTypes.hpp"
#include "disassembler/Disassembler.hpp"
#include "translator/ArmFile.hpp"

int main() {
    while (true) {
        std::string fileName = "";

        std::cout << "Enter file name: ";
        std::cin >> fileName;

        if (fileName == "q" || fileName == "") break;

        system("clear");

        auto riscFile =
            Disassembler::disassemble("data/test/elf/" + fileName + ".elf");

        std::cout << "<<<<<<<<<<<<<<<<RISC-V File>>>>>>>>>>>>>>>>\n";

        std::cout << riscFile->toString();

        std::cout
            << "<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>\n\n<<<<<<<<<<<<<<<<"
               "ARM64 File>>>>>>>>>>>>>>>>\n";

        auto armFile = Translator::ArmFile(riscFile);

        std::cout << armFile.toString();

        std::cout << "<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>\n";

        riscFile->writeToFile(fileName);
        armFile.writeToFile(fileName);

        Analyser::analyse(armFile);

        auto& analysisReport = armFile.getAnalysis();

        std::cout
            << "<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>\n\n<<<<<<<<<<<<<<<<"
               "Analysis>>>>>>>>>>>>>>>>\n"
            << Analyser::to_string(*analysisReport)
            << "<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>\n";
    }

    return 0;
}

// NEXT catch errors for nice printing