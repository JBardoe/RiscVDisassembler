#include "disassembler/Parser.hpp"

std::unique_ptr<ELFFile> parseFile(const std::string& filepath) {
    ELFIO::elfio temp = ELFIO::elfio();

    if (!temp.load(filepath)) {
        std::cout << "File not found\n";
        return nullptr;
    } else if (temp.get_class() != ELFIO::ELFCLASS32) {
        std::cout << "File is not 32 bit\n";
        return nullptr;
    } else if (temp.get_machine() != ELFIO::EM_RISCV) {
        std::cout << "File is not RISC-V\n";
        return nullptr;
    } else if (temp.sections[".text"] == nullptr) {
        std::cout << "No text section\n";
        return nullptr;
    }

    std::unique_ptr<ELFFile> file = std::make_unique<ELFFile>(
        std::move(std::make_unique<ELFIO::elfio>(temp)));

    return std::move(std::make_unique<ELFFile>(file));
}