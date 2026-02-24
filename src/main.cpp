#include <cstdint>
#include <iomanip>
#include <iostream>

#include "disassembler/Disassembler.hpp"
#include "parser/ELFFile.hpp"
#include "parser/Parser.hpp"

int main() {
    auto asmFile = Disassembler::disassemble("data/test/elf/multipleEntry.elf");

    std::cout << asmFile->toString();

    return 0;
}