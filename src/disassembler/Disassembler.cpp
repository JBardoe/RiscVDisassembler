#include "disassembler/Disassembler.hpp"

#include <iostream>
#include <optional>

#include "disassembler/ELFFile.hpp"
#include "disassembler/Parser.hpp"

using namespace std;

unique_ptr<AssemblyFile> disassemble(const string& filepath) {
    unique_ptr<ELFParser::ELFFile> elffile = ELFParser::parseFile(filepath);
    if (!elffile) return nullptr;

    return nullptr;
}