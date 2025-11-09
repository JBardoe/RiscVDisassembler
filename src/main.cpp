#include <elfio/elfio.hpp>
#include <iomanip>
#include <iostream>

#include "disassembler/Disassembler.hpp"

int main(int argc, char* argv[]) {
    std::cout << "About to parse the file\n";
    std::optional<ELFIO::elfio> file = get_file("data/elf/add.elf");
    if (file.has_value()) {
        std::cout << "Parsed the file\n";
        const char* data = file.value().sections[".text"]->get_data();
        ELFIO::Elf_Xword size = file.value().sections[".text"]->get_size();

        std::cout << ".text section (" << size << " bytes):" << std::endl;
        for (ELFIO::Elf_Xword i = 0; i < size; ++i) {
            if (i % 16 == 0)
                std::cout << std::endl
                          << std::setw(8) << std::setfill('0') << std::hex << i
                          << ": ";
            std::cout << std::setw(2) << std::setfill('0') << std::hex
                      << (static_cast<unsigned>(
                              static_cast<unsigned char>(data[i])) &
                          0xff)
                      << ' ';
        }
    }

    return 0;
}
