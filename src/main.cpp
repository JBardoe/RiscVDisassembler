#include <stdlib.h>

#include <cstdint>
#include <iomanip>
#include <iostream>
#include <string>

#include "disassembler/Disassembler.hpp"
#include "parser/ELFFile.hpp"
#include "parser/Parser.hpp"
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
    }

    return 0;
}

// TODO catch errors for nice printing