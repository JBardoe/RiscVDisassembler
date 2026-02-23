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
    if (opcodeMap.find(raw & 0x7F) == opcodeMap.end()) {
        throw DisassemblyException("Unknown Opcode: " +
                                   format("{:02X}", (raw & 0x7F)));
    }

    Opcode opcode = opcodeMap.at(raw & 0x7F);

    switch (opcode) {
        // R-Type
        case Opcode::R_TYPE:
            return make_unique<RInstruction>(opcode, raw);

        // I-Types
        case Opcode::LOAD:
        case Opcode::IMM_INSTR:
        case Opcode::JALR:
        case Opcode::ENV_TYPE:
            return make_unique<IInstruction>(opcode, raw);

        // S-Type
        case Opcode::S_TYPE:
            return make_unique<SInstruction>(opcode, raw);

        // B-Type
        case Opcode::B_TYPE:
            return make_unique<BInstruction>(opcode, raw);

        // U-Type
        case Opcode::AUIPC:
        case Opcode::LUI:
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

    uint32_t offset = 0;

    uint32_t gpAddress = 0;

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
                    offset++;
                    continue;
                }

                if (symbolData[offset].name >
                    sections.at(".strtab")->header->size) {
                    throw ELFParser::BadFileException(
                        "Invalid symbol table entry.");
                }

                string symbolName(reinterpret_cast<const char*>(
                    stringTable + symbolData[offset].name));

                string sectionName =
                    elffile->getSectionName(symbolData[offset].shndx);

                asmFile->addSymbol(
                    symbolName, symbolData[offset].value,
                    symbolData[offset].size,
                    static_cast<SymbolType>(symbolData[offset].info & 0x0F),
                    static_cast<SymbolBinding>(symbolData[offset].info >> 4),
                    sectionName);

                if (symbolName == "__global_pointer$") {
                    gpAddress = symbolData[offset].value;
                }

                offset++;
            }
        }
    }

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
    offset = 0;

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

    // uint32_t dataSectionAddress;
    auto dataIt = sections.find(".data");

    // If there's no .data section, skip the code that needs ut
    if (dataIt == sections.end()) {
        goto nodata;
    }

    // dataSectionAddress = sections.at(".data")->header->offset;

    if (gpAddress != 0) {
        for (auto& instr : textInstructions) {
            auto* casted = dynamic_cast<IInstruction*>(instr.get());
            if (casted && casted->instr == Operator::addi &&
                casted->rs1 == Register::gp) {
                uint32_t symbolAddress = gpAddress + casted->imm;
                auto var = asmFile->getSymbolAddr(symbolAddress);

                if (var.empty()) continue;

                instr = move(make_unique<PseudoLoadInstruction>(
                    Operator::la, casted->rd, var[0].name));
            }
        }
    }

    // TODO deal with PC relative addressing
    // TODO add data section

nodata:

    asmFile->addSection(".text",
                        move(make_unique<TextSection>(move(textInstructions))));

    asmFile->addSection(".data", move(make_unique<DataSection>()));

    cout << "Sections:\n";

    for (auto& sec : sections) {
        cout << "Name: " << sec.first
             << ". Offset: " << sec.second->header->offset
             << ". Size: " << sec.second->header->size << "\n";
    }

    return asmFile;
}
}  // namespace Disassembler
