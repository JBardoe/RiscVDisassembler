#include "disassembler/Parser.hpp"

#include <fstream>
#include <iostream>
#include <memory>

#include "disassembler/ELFTypes.hpp"
#include "utils/BadFileException.hpp"
using namespace std;

unique_ptr<ELFFile> parseFile(const string& filepath) {
    unique_ptr<ELFFile> file = make_unique<ELFFile>();

    if (!(filepath.substr(filepath.length() - 4, 4).compare(".elf") == 0))
        throw BadFileException("File not in ELF format");

    ifstream filestream;

    filestream.open(filepath, ifstream::in | ifstream::binary);

    if (filestream.fail()) throw BadFileException("Failed to open file");

    parseHeader(filestream, *file);

    filestream.close();
    return move(file);
}

void parseHeader(ifstream& filestream, const ELFFile& file) {
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

    ELFHeader header;

    filestream.seekg(0);
    filestream.read(reinterpret_cast<char*>(&header), sizeof(header));

    if (filestream.gcount() != sizeof(header))
        throw BadFileException("Invalid file length");
    if (header.fileType < 1 || header.fileType > 3)
        throw BadFileException("Invalid file type");
    if (header.architecture != 243)
        throw BadFileException("File is not RISC-V");
    if (header.elfVersion != 1) throw BadFileException("Incorrect ELF version");
    if (header.headerSize != sizeof(header))
        throw BadFileException("Incorrect header size");
}