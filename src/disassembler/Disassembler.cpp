#include "disassembler/Disassembler.hpp"

#include <cstdint>
#include <format>
#include <iostream>
#include <vector>

#include "disassembler/Instruction.hpp"
#include "disassembler/RiscvTypes.hpp"
#include "parser/ELFFile.hpp"
#include "parser/ELFTypes.hpp"
#include "parser/Parser.hpp"
#include "utils/BadFileException.hpp"
#include "utils/DisassemblyException.hpp"

using namespace std;

namespace Disassembler {

unique_ptr<Instruction> parseInstruction(uint32_t raw) {
    // Get opcode
    if (Disassembler::opcodeMap.find(raw & 0x7F) ==
        Disassembler::opcodeMap.end()) {
        throw DisassemblyException("Unknown Disassembler::Opcode: " +
                                   format("{:02X}", (raw & 0x7F)));
    }

    Disassembler::Opcode opcode = Disassembler::opcodeMap.at(raw & 0x7F);

    switch (opcode) {
        // R-Type
        case Disassembler::Opcode::R_TYPE:
            return make_unique<RInstruction>(opcode, raw);

        // I-Types
        case Disassembler::Opcode::LOAD:
        case Disassembler::Opcode::IMM_INSTR:
        case Disassembler::Opcode::JALR:
        case Disassembler::Opcode::ENV_TYPE:
            return make_unique<IInstruction>(opcode, raw);

        // S-Type
        case Disassembler::Opcode::S_TYPE:
            return make_unique<SInstruction>(opcode, raw);

        // B-Type
        case Disassembler::Opcode::B_TYPE:
            return make_unique<BInstruction>(opcode, raw);

        // U-Type
        case Disassembler::Opcode::AUIPC:
        case Disassembler::Opcode::LUI:
            return make_unique<UInstruction>(opcode, raw);

        // J-Type
        default:
            return make_unique<JInstruction>(opcode, raw);
    }
}

unique_ptr<AssemblyFile> disassemble(const string& filepath) {
    unique_ptr<ELFParser::ELFFile> elffile = ELFParser::parseFile(filepath);
    if (!elffile) return nullptr;

    auto asmFile = make_unique<AssemblyFile>();

    const unordered_map<string, unique_ptr<ELFParser::ELFSection>>& sections =
        elffile->getSections();

    // Decode .text section
    if (sections.find(".text") == sections.end()) {
        throw ELFParser::BadFileException("No text section found.");
    }

    if (sections.at(".text")->header->size % 4 != 0) {
        throw ELFParser::BadFileException(
            "Text section is of an invalid size.");
    }

    const unsigned char* textData =
        reinterpret_cast<const unsigned char*>(sections.at(".text")->getData());
    vector<unique_ptr<Instruction>> textInstructions;

    uint32_t offset = 0;

    while (offset < sections.at(".text")->header->size) {
        uint32_t instr;
        if (elffile->isLittleEndian) {
            instr = (textData[offset]) | (textData[offset + 1] << 8) |
                    (textData[offset + 2] << 16) | (textData[offset + 3] << 24);
        } else {
            instr = (textData[offset + 3]) | (textData[offset + 2] << 8) |
                    (textData[offset + 1] << 16) | (textData[offset] << 24);
        }
        textInstructions.push_back(move(parseInstruction(instr)));
        offset += 4;
    }

    asmFile->addSection(".text",
                        move(make_unique<TextSection>(move(textInstructions))));

    // Parse symbol tables if they exists
    for (auto& sec : sections) {
        if (sec.second->header->type == 11 || sec.second->header->type == 2) {
            if (sections.find(".strtab") == sections.end()) {
                throw ELFParser::BadFileException("Missing string table.");
            }

            const unsigned char* stringTable =
                reinterpret_cast<const unsigned char*>(
                    sections.at(".strtab")->getData());

            if (sec.second->header->entrySize !=
                    sizeof(ELFParser::SymbolTableEntry) ||
                sec.second->header->size % sec.second->header->entrySize != 0) {
                throw ELFParser::BadFileException("Malformed symbol table.");
            }

            const ELFParser::SymbolTableEntry* symbolData =
                reinterpret_cast<const ELFParser::SymbolTableEntry*>(
                    sec.second
                        ->getData());  // TODO: check if needs to be freed?

            offset = 0;

            while (offset * sec.second->header->entrySize <
                   sec.second->header->size) {
                if (symbolData[offset].name < 1) {
                    offset++;
                    continue;
                }

                if (symbolData[offset].name >
                    sections.at(".strtab")->header->size) {
                    throw ELFParser::BadFileException(
                        "Invalid symbol table entry.");
                }

                std::string symbolName(reinterpret_cast<const char*>(
                    stringTable + symbolData[offset].name));

                std::string sectionName =
                    elffile->getSectionName(symbolData[offset].shndx);

                asmFile->addSymbol(symbolName,
                                   make_unique<Disassembler::Symbol>(
                                       symbolName, symbolData[offset].value,
                                       symbolData[offset].size,
                                       static_cast<Disassembler::SymbolType>(
                                           symbolData[offset].info & 0x0F),
                                       static_cast<Disassembler::SymbolBinding>(
                                           symbolData[offset].info >> 4),
                                       sectionName),
                                   sec.second->header->type);

                offset++;
            }
        }
    }

    std::cout << "Sections:\n";

    for (auto& sec : sections) {
        std::cout << "Name: " << sec.first
                  << ". Offset: " << sec.second->header->offset
                  << ". Size: " << sec.second->header->size << "\n";
    }

    std::cout << "\n\nSymbol Table:\n";

    for (auto& sym : asmFile->getSymbolTable()) {
        std::cout << "\tName: " << sym.first
                  << " | Address: " << sym.second->addr
                  << " | Type: " << to_string(sym.second->type)
                  << " | Binding: " << to_string(sym.second->binding)
                  << " | Section: " << sym.second->sectionName << "\n";
    }

    return asmFile;
}
}  // namespace Disassembler
