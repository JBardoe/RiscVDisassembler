#include "disassembler/Parser.hpp"

#include <fstream>
#include <iostream>
#include <memory>

#include "utils/BadFileException.hpp"
using namespace std;

unique_ptr<ELFFile> parseFile(const string& filepath) {
    unique_ptr<ELFFile> file = make_unique<ELFFile>();

    if (!(filepath.substr(filepath.length() - 4, 4).compare(".elf") == 0))
        throw BadFileException("File not in ELF format");

    ifstream filestream;

    filestream.open(filepath, ifstream::in | ifstream::binary);

    if (filestream.fail()) throw BadFileException("Failed to open file");

    array<char, 16> identifiers = {0};

    filestream.read(identifiers.data(), 16);

    if (filestream.gcount() != 16)
        throw BadFileException("Invalid file length");
    if (static_cast<unsigned char>(identifiers[0]) != 0x7F ||
        identifiers[1] != 'E' || identifiers[2] != 'L' || identifiers[3] != 'F')
        throw BadFileException("Failed to parse ELF magic bytes");

    if (identifiers[4] != 1) throw BadFileException("File is not 32-bit");
    if (identifiers[5] != 1 && identifiers[5] != 2)
        throw BadFileException("Invalid ELF header");
    if (identifiers[6] != 1) throw BadFileException("Invalid ELF version");

    parseHeader(filestream, *file);

    filestream.close();
    return move(file);
}

void parseHeader(const ifstream& filestream, const ELFFile& file) {}
