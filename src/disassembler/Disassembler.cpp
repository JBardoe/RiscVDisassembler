#include "disassembler/Disassembler.hpp"

#include <cstdint>
#include <format>
#include <iostream>
#include <vector>

#include "disassembler/ELFFile.hpp"
#include "disassembler/ELFTypes.hpp"
#include "disassembler/Instruction.hpp"
#include "disassembler/Parser.hpp"
#include "disassembler/RiscvTypes.hpp"
#include "utils/BadFileException.hpp"
#include "utils/DisassemblyException.hpp"

using namespace std;

namespace Disassembler {

unique_ptr<Instruction> parseInstruction(uint32_t raw) {
    // Get opcode
    if (RISCV::opcodeMap.find(raw & 0x7F) == RISCV::opcodeMap.end()) {
        throw DisassemblyException("Unknown RISCV::Opcode: " +
                                   format("{:02X}", (raw & 0x7F)));
    }

    RISCV::Opcode opcode = RISCV::opcodeMap.at(raw & 0x7F);

    switch (opcode) {
        // R-Type
        case RISCV::Opcode::R_TYPE:
            return make_unique<RInstruction>(opcode, raw);

        // I-Types
        case RISCV::Opcode::LOAD:
        case RISCV::Opcode::IMM_INSTR:
        case RISCV::Opcode::JALR:
        case RISCV::Opcode::ENV_TYPE:
            return make_unique<IInstruction>(opcode, raw);

        // S-Type
        case RISCV::Opcode::S_TYPE:
            return make_unique<SInstruction>(opcode, raw);

        // B-Type
        case RISCV::Opcode::B_TYPE:
            return make_unique<BInstruction>(opcode, raw);

        // U-Type
        case RISCV::Opcode::AUIPC:
        case RISCV::Opcode::LUI:
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
                    sec.second->getData());

            offset = 0;

            while (offset * sec.second->header->entrySize <
                   sec.second->header->size) {
                if (symbolData[offset].name < 1) {
                    if (symbolData[offset].name >
                        sections.at(".strtab")->header->size) {
                        throw ELFParser::BadFileException(
                            "Invalid symbol table entry.");
                    }
                    if (symbolData[offset].name & 0x0F != 1 &&
                        symbolData[offset].name & 0x0F != 1) {
                        offset++;
                        continue;
                    }

                    std::string symbolName(reinterpret_cast<const char*>(
                        stringTable + symbolData[offset].name));

                    const std::string& sectionName =
                        elffile->getSectionName(symbolData[offset].shndx);
                    ELFParser::ELFSection* section =
                        (sectionName == "" ||
                         sections.find(sectionName) == sections.end())
                            ? nullptr
                            : sections.at(sectionName).get();

                    asmFile->addSymbol(symbolName,
                                       make_unique<RISCV::Symbol>(
                                           symbolName, symbolData[offset].value,
                                           symbolData[offset].size,
                                           static_cast<RISCV::SymbolType>(
                                               symbolData[offset].info >> 4),
                                           static_cast<RISCV::SymbolBinding>(
                                               symbolData[offset].info & 0x0F),
                                           section),
                                       sec.second->header->type);
                }
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

    return asmFile;
}
}  // namespace Disassembler