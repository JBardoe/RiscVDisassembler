#include "disassembler/Disassembler.hpp"

#include <iostream>
#include <optional>

#include "disassembler/ELFFile.hpp"
#include "disassembler/Parser.hpp"

using namespace std;

unique_ptr<AssemblyFile> disassemble(const string& filepath) {
    unique_ptr<ELFFile> elffile = parseFile(filepath);
    if (!elffile) return nullptr;

    return nullptr;
}