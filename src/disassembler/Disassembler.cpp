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

unique_ptr<Instruction> parseInstruction(uint32_t raw, uint32_t offset) {
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
            return make_unique<UInstruction>(opcode, raw, offset);

        // J-Type
        default:
            return make_unique<JInstruction>(opcode, raw);
    }
}

unique_ptr<DataSection> disassembleDataSection(
    const unique_ptr<AssemblyFile>& asmFile,
    const unique_ptr<ELFParser::ELFSection>& dataSection, bool isLittleEndian) {
    auto data = make_unique<DataSection>();

    uint32_t dataBase = dataSection->header->offset;
    uint32_t dataUpper = dataBase + dataSection->header->size;

    auto vars = asmFile->getSymbolSection(".data");

    if (vars.empty()) return data;

    sort(vars.begin(), vars.end(),
         [](auto& a, auto& b) { return a.addr < b.addr; });

    const char* dataData = dataSection->getData();

    string current = vars[0].name;  // TODO change
    uint32_t currentAddress = vars[0].addr;
    uint32_t currentSize;
    uint32_t currentVal;
    uint32_t offset;

    for (size_t i = 1; i < vars.size(); i++) {
        if (vars[i].addr != currentAddress) {
            currentVal = 0;

            offset = currentAddress - dataBase;
            currentSize = vars[i].addr - currentAddress;

            if (currentSize < 1 || currentSize > 4) {
                throw DisassemblyException("Invalid Symbol Size: " +
                                           std::to_string(currentSize));
            }

            // Get all the required bytes out of the file
            for (uint32_t j = 0; j < currentSize; j++) {
                if (isLittleEndian) {
                    currentVal |= (dataData[offset + j] << (j * 8));
                } else {
                    currentVal |=
                        (dataData[offset + j] << ((currentSize - j - 1) * 8));
                }
            }

            data->addVariable(current, currentAddress, currentVal,
                              currentSize);  // Currently erasing other symbols
                                             // with same address as they will
                                             // never be emitted anyway

            currentAddress = vars[i].addr;
            current = vars[i].name;
        }
    }

    if (currentAddress < dataUpper) {
        currentVal = 0;

        offset = currentAddress - dataBase;
        currentSize = dataUpper - currentAddress;

        if (currentSize < 1 || currentSize > 4) {
            throw DisassemblyException("Invalid Symbol Size: " +
                                       std::to_string(currentSize));
        }

        // Get all the required bytes out of the file
        for (uint32_t j = 0; j < currentSize; j++) {
            if (isLittleEndian) {
                currentVal |= (dataData[offset + j] << (j * 8));
            } else {
                currentVal |=
                    (dataData[offset + j] << ((currentSize - j - 1) * 8));
            }
        }

        data->addVariable(current, currentAddress, currentVal,
                          currentSize);  // Currently erasing other symbols
                                         // with same address as they will
                                         // never be emitted anyway
    }

    return data;
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

    if (auto dataIt = sections.find(".data"); dataIt != sections.end()) {
        auto dataSection = disassembleDataSection(asmFile, (*dataIt).second,
                                                  elffile->isLittleEndian);
        asmFile->addSection(".data", move(dataSection));
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
        textInstructions.push_back(move(parseInstruction(
            instr, sections.at(".text")->header->address + offset)));
        offset += 4;
    }

    size_t i = 0;
    // Translate unravelled pseudo-intructions that use symbols
    while (i < textInstructions.size()) {
        auto& instr = textInstructions[i];
        uint32_t symbolAddress;
        vector<Disassembler::Symbol> vars;

        // If it uses a gp relative address, convert from that
        if (gpAddress != 0) {
            auto* castedI = dynamic_cast<IInstruction*>(instr.get());
            if (castedI && castedI->instr == Operator::addi &&
                castedI->rs1 == Register::gp) {
                symbolAddress = gpAddress + castedI->imm;
                vars = asmFile->getSymbolAddr(symbolAddress);

                if (vars.empty()) {
                    i++;
                    continue;
                }

                textInstructions[i] = move(make_unique<PseudoLoadInstruction>(
                    Operator::la, castedI->rd, vars[0].name));
                i++;
                continue;
            }
        }

        // If it uses pc-relative address, convert from that
        if (instr->instr == Operator::auipc &&
            i < textInstructions.size() - 1 &&
            textInstructions[i + 1]->instr == Operator::addi) {
            auto* castedAUIPC = dynamic_cast<UInstruction*>(instr.get());
            auto* castedADDI =
                dynamic_cast<IInstruction*>(textInstructions[i + 1].get());

            if (castedADDI && castedAUIPC &&
                castedADDI->rd == castedAUIPC->rd &&
                castedADDI->rd == castedADDI->rs1) {
                symbolAddress = castedAUIPC->addr + (castedAUIPC->imm << 12) +
                                castedADDI->imm;

                vars = asmFile->getSymbolAddr(symbolAddress);

                if (vars.empty()) {
                    i++;
                    continue;
                }

                textInstructions[i] = move(make_unique<PseudoLoadInstruction>(
                    Operator::la, castedADDI->rd, vars[0].name));
                textInstructions.erase(textInstructions.begin() + i + 1,
                                       textInstructions.begin() + i + 2);
            }
        }
        i++;
    }

    asmFile->addSection(".text",
                        move(make_unique<TextSection>(move(textInstructions))));

    // TODO remove
    cout << "Sections:\n";

    for (auto& sec : sections) {
        cout << "Name: " << sec.first
             << ". Offset: " << sec.second->header->offset
             << ". Size: " << sec.second->header->size << "\n";
    }
    cout << "\n";

    return asmFile;
}
}  // namespace Disassembler
