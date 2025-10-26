#include "disassembler/Disassembler.hpp"

#include <elfio/elfio.hpp>
#include <iostream>
#include <optional>

std::optional<ELFIO::elfio> get_file(const std::string& filepath) {
    ELFIO::elfio file = ELFIO::elfio();

    if (!file.load(filepath)) {
        std::cout << "File not found\n";
        return std::nullopt;
    } else if (file.get_class() != ELFIO::ELFCLASS32) {
        std::cout << "File is not 32 bit\n";
        return std::nullopt;
    } else if (file.get_machine() != ELFIO::EM_RISCV) {
        std::cout << "File is not RISC-V\n";
        return std::nullopt;
    } else if (file.sections[".text"] == nullptr) {
        std::cout << "No text section\n";
        return std::nullopt;
    }

    return file;
}

std::optional<AssemblyFile> disassemble(const std::string& filepath) {
    std::optional<ELFIO::elfio> maybeFile = get_file(filepath);
    if (!maybeFile.has_value()) return std::nullopt;
    ELFIO::elfio& file = maybeFile.value();
    return std::nullopt;
}