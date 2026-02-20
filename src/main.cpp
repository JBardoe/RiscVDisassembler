#include <cstdint>
#include <iomanip>
#include <iostream>

#include "disassembler/Disassembler.hpp"
#include "parser/ELFFile.hpp"
#include "parser/Parser.hpp"

int main() {
    std::cout << "About to disassemble the file\n";

    auto asmFile = Disassembler::disassemble("data/test/elf/add.elf");

    if (asmFile) {
        std::cout << "Disassembled file\n\n\n";
    }

    std::cout << asmFile->toString();

    return 0;
}