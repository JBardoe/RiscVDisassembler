#include <stdlib.h>

#include <cstdint>
#include <iomanip>
#include <iostream>
#include <string>

#include "disassembler/Disassembler.hpp"
#include "parser/ELFFile.hpp"
#include "parser/Parser.hpp"

int main() {
    while (true) {
        std::string fileName = "";

        std::cout << "Enter file name: ";
        std::cin >> fileName;

        if (fileName == "q" || fileName == "") break;

        system("clear");

        auto asmFile =
            Disassembler::disassemble("data/test/elf/" + fileName + ".elf");

        std::cout << asmFile->toString();
    }

    return 0;
}