#include <cstdint>
#include <iomanip>
#include <iostream>

#include "disassembler/Disassembler.hpp"
#include "disassembler/ELFFile.hpp"
#include "disassembler/Parser.hpp"

int main() {
    std::cout << "About to disassemble the file\n";

    auto asmFile = Disassembler::disassemble("data/test/elf/add.elf");

    if (asmFile) {
        std::cout << "Disassembled file\n\n\n";
    }

    std::cout << asmFile->toString();

    return 0;
}