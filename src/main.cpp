#include <cstdint>
#include <iomanip>
#include <iostream>

#include "disassembler/Disassembler.hpp"
#include "disassembler/ELFFile.hpp"
#include "disassembler/Parser.hpp"

int main() {
    std::cout << "About to parse the file\n";
    auto file = ELFParser::parseFile("data/elf/add.elf");
    if (file) {
        std::cout << "Parsed the file\n";
    }

    const char* textData = (file->getSections()[".data"])->getData();
    uint32_t size = (file->getSections()[".data"])->getHeader().size;

    for (auto& sect : file->getSections()) {
        std::cout << "Section:" << sect.first << "\n";
    }

    for (uint32_t i = 0; i < size; ++i) {
        if (i % 16 == 0)
            std::cout << std::endl
                      << std::setw(8) << std::setfill('0') << std::hex << i
                      << ": ";
        std::cout << std::setw(2) << std::setfill('0') << std::hex
                  << (static_cast<unsigned>(
                          static_cast<unsigned char>(textData[i])) &
                      0xff)
                  << ' ';
    }

    std::cout << "\n";

    return 0;
}