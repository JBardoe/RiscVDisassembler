#include "disassembler/Disassembler.hpp"

#include <format>
#include <iostream>
#include <utility>
#include <vector>

#include "disassembler/RiscvInstruction.hpp"
#include "disassembler/RiscvTypes.hpp"
#include "parser/ELFFile.hpp"
#include "parser/ELFTypes.hpp"
#include "parser/Parser.hpp"
#include "utils/AssemblySection.hpp"
#include "utils/BadFileException.hpp"
#include "utils/DisassemblyException.hpp"

using namespace std;

namespace Disassembler {

unique_ptr<RiscvInstruction> parseInstruction(uint32_t raw, uint32_t offset) {
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

void disassembleSymbolTable(
    const unique_ptr<RiscvFile>& asmFile,
    const unique_ptr<ELFParser::ELFFile>& elffile,
    const unordered_map<string, unique_ptr<ELFParser::ELFSection>>& sections) {
    uint32_t offset = 0;

    // Parse symbol tables if they exists
    for (auto& sec : sections) {
        if (sec.second->header->type == 11 ||
            sec.second->header->type ==
                2) {  // Find symbol tables based on section type as names can
                      // be unreliable

            // If the string table does not exist, no symbols can be decoded
            if (sections.find(".strtab") == sections.end()) {
                throw ELFParser::BadFileException("Missing string table.");
            }

            // Get and check the string table
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

            // For every entry in the symbol table, if it is useful, add it
            while (offset * sec.second->header->entrySize <
                   sec.second->header->size) {
                if (symbolData[offset].name <
                    1) {  // Unnamed symbols can exist and are not relevant
                    offset++;
                    continue;
                }

                if (symbolData[offset].name >
                    sections.at(".strtab")->header->size) {
                    throw ELFParser::BadFileException(
                        "Invalid symbol table entry.");
                }

                // Get the symbol data and add to the disassembled symbol table
                string symbolName(reinterpret_cast<const char*>(
                    stringTable + symbolData[offset].name));

                string sectionName =
                    elffile->getSectionName(symbolData[offset].shndx);

                asmFile->addSymbol(
                    symbolName, symbolData[offset].value,
                    symbolData[offset].size,
                    static_cast<SymbolType>(symbolData[offset].info & 0x0F),
                    static_cast<Assembly::SymbolBinding>(
                        symbolData[offset].info >> 4),
                    sectionName);

                offset++;
            }
        }
    }
}

shared_ptr<Assembly::DataSection> disassembleDataSection(
    const unique_ptr<RiscvFile>& asmFile,
    const unique_ptr<ELFParser::ELFSection>& dataSection, bool isLittleEndian) {
    auto data = make_shared<Assembly::DataSection>();

    // Base address of variables in .data section (can be an index or a virtual
    // address)
    uint32_t dataBase = (asmFile->getFileType() == FileType::REL)
                            ? 0
                            : dataSection->header->address;

    // Upper bound on the .data section
    uint32_t dataUpper = dataBase + dataSection->header->size;

    // Get all symbols from the symbol table that are in the .data section and
    // return an empty data section if there are none
    auto vars = asmFile->getSymbolSection(".data");

    if (vars.empty()) return data;

    // Sort the symbols by their address in the .data section so their size can
    // be determined as the gap between the current and next symbol
    sort(vars.begin(), vars.end(),
         [](auto& a, auto& b) { return a.addr < b.addr; });

    const unsigned char* dataData =
        reinterpret_cast<const unsigned char*>(dataSection->getData());

    string current = vars[0].name;
    uint32_t currentAddress = vars[0].addr;
    uint32_t currentSize;
    uint32_t currentVal;
    uint32_t offset;

    // For each symbol in the .data section
    for (size_t i = 1; i < vars.size(); i++) {
        // If the next address has been reached (indicates the size of the
        // previous symbol can now be determined)
        if (vars[i].addr != currentAddress) {
            currentVal = 0;

            offset = currentAddress - dataBase;
            currentSize = vars[i].addr - currentAddress;

            if (currentSize < 1 || currentSize > 4) {
                throw DisassemblyException("Invalid Symbol Size: " +
                                           std::to_string(currentSize));
            }

            // Get all the required bytes out of the raw data according to the
            // endianness
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

    // Add the last variable to the section if it is valid
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

shared_ptr<Assembly::BSSSection> disassembleBSSSection(
    const unique_ptr<RiscvFile>& asmFile,
    const unique_ptr<ELFParser::ELFSection>& bssSection) {
    auto bss = make_shared<Assembly::BSSSection>();

    // Base address of variables in .bss section (can be an index or a virtual
    // address)
    uint32_t bssBase = (asmFile->getFileType() == FileType::REL)
                           ? 0
                           : bssSection->header->address;

    // Upper bound on the .bss section
    uint32_t bssUpper = bssBase + bssSection->header->size;

    // Get all symbols from the symbol table that are in the .bss section and
    // return an empty .bss section if there are none
    auto vars = asmFile->getSymbolSection(".bss");

    if (vars.empty()) return bss;

    // Sort the symbols by their address in the .bss section so their size can
    // be determined as the gap between the current and next symbol
    sort(vars.begin(), vars.end(),
         [](auto& a, auto& b) { return a.addr < b.addr; });

    // Eliminate common .bss symbols that are not variables
    while (vars[0].name == "__bss_start" || vars[0].name == "__BSS_END__" ||
           vars[0].name == "_end" || vars[0].name == "_edata") {
        vars.erase(vars.begin(), vars.begin() + 1);
    }

    string current = vars[0].name;
    uint32_t currentAddress = vars[0].addr;
    uint32_t currentSize;

    // For each symbol in the .bss section
    for (size_t i = 1; i < vars.size(); i++) {
        // Eliminate common .bss symbols that are not variables
        if (vars[i].name == "__bss_start" || vars[i].name == "__BSS_END__" ||
            vars[i].name == "_end" || vars[i].name == "_edata") {
            continue;
        }

        // If the next address has been reached (indicates the size of the
        // previous symbol can now be determined)
        if (vars[i].addr != currentAddress) {
            currentSize = vars[i].addr - currentAddress;

            if (currentSize < 1 || currentSize > 4) {
                throw DisassemblyException("Invalid Symbol Size: " +
                                           std::to_string(currentSize));
            }

            bss->addVariable(current, currentAddress,
                             currentSize);  // Currently erasing other symbols
                                            // with same address as they will
                                            // never be emitted anyway

            currentAddress = vars[i].addr;
            current = vars[i].name;
        }
    }

    // Add the last variable to the section if it is valid
    if (currentAddress < bssUpper) {
        currentSize = bssUpper - currentAddress;

        if (currentSize < 1 || currentSize > 4) {
            throw DisassemblyException("Invalid Symbol Size: " +
                                       std::to_string(currentSize));
        }

        bss->addVariable(current, currentAddress,
                         currentSize);  // Currently erasing other symbols
                                        // with same address as they will
                                        // never be emitted anyway
    }

    return bss;
}

shared_ptr<TextSection> disassembleTextSection(
    const unique_ptr<RiscvFile>& asmFile,
    const unique_ptr<ELFParser::ELFSection>& textSection, uint32_t gpAddress,
    bool isLittleEndian) {
    // Instructions are all 4 bytes so if the .text section is not a multiple of
    // 4, there is an issue
    if (textSection->header->size % 4 != 0) {
        throw ELFParser::BadFileException(
            "Text section is of an invalid size.");
    }

    const unsigned char* textData =
        reinterpret_cast<const unsigned char*>(textSection->getData());
    vector<unique_ptr<RiscvInstruction>> textInstructions;
    uint32_t offset = 0;

    // For each instruction in the section, extract it, construct a uint32_t
    // value based on endianness, and dispatch to the decoding method
    while (offset < textSection->header->size) {
        uint32_t instr;
        if (isLittleEndian) {
            instr = (textData[offset]) | (textData[offset + 1] << 8) |
                    (textData[offset + 2] << 16) | (textData[offset + 3] << 24);
        } else {
            instr = (textData[offset + 3]) | (textData[offset + 2] << 8) |
                    (textData[offset + 1] << 16) | (textData[offset] << 24);
        }
        textInstructions.push_back(move(
            parseInstruction(instr, textSection->header->address + offset)));
        offset += 4;
    }

    auto entries = asmFile->getSymbolSection(".text");
    vector<pair<string, Assembly::SymbolBinding>> entryPoints;

    int added = 0;

    // Extract all entry points using the symbol table
    for (size_t i = 0; i < entries.size(); i++) {
        auto entry = entries[i];
        uint32_t entryAddr =
            entry.addr - ((asmFile->getFileType() == FileType::REL)
                              ? 0
                              : textSection->header->address);

        if (entryAddr / 4 > textInstructions.size() - added)
            continue;  // Must be divisible by 4 (enforced in instruction
                       // constructor)

        textInstructions.insert(
            textInstructions.begin() + (entryAddr / 4) + added,
            make_unique<EntryPoint>(entry.name));
        entryPoints.push_back(make_pair(entry.name, entry.binding));
        added++;
    }

    unordered_map<int, string>
        tmps;  // New entry points created to remove relative jumps/branches

    int tmpCount = 0;

    // Turn each jump/branch that uses an address to the equivalent entry point
    // If it does not go to an entry points, add one for it to use (helps with
    // translation)
    for (size_t i = 0; i < textInstructions.size(); i++) {
        if (textInstructions[i]->instr ==
            Operator::jal) {  // Translate explicit jumps
            auto* castedJ =
                dynamic_cast<JInstruction*>(textInstructions[i].get());
            if (!castedJ) continue;

            size_t offset =
                max(i + (castedJ->imm / 4), static_cast<unsigned long>(1));
            if (offset >= textInstructions.size()) continue;
            if (castedJ->imm < 0) offset--;

            // If there's already an entry point at the end of the jump
            if (auto* castedE =
                    dynamic_cast<EntryPoint*>(textInstructions[offset].get())) {
                textInstructions[i] = move(
                    make_unique<JInstructionEntry>(castedJ, castedE->name));
            } else if (auto it = tmps.find(offset);
                       it != tmps.end()) {  // If a temp entry point has
                                            // already been added
                textInstructions[i] =
                    move(make_unique<JInstructionEntry>(castedJ, it->second));
            } else {  // Otherwise add a new entry points

                // Check if there's already a symbol with the same name as the
                // new temp entry point (Unlikely to trigger)
                while (asmFile->getSymbolName(
                           "L.diss." + std::to_string(tmpCount)) != nullopt)
                    tmpCount++;

                // Add a new entry point
                tmps.insert({offset, "L.diss." + std::to_string(tmpCount)});
                entryPoints.push_back(
                    make_pair("L.diss." + std::to_string(tmpCount),
                              Assembly::SymbolBinding::LOCAL));
                textInstructions[i] = move(make_unique<JInstructionEntry>(
                    castedJ, "L.diss." + std::to_string(tmpCount)));
                tmpCount++;
            }
        } else if (textInstructions[i]->op ==
                   Opcode::B_TYPE) {  // Translate explicit branches
            auto* castedB =
                dynamic_cast<BInstruction*>(textInstructions[i].get());
            if (!castedB) continue;

            size_t offset =
                max(i + (castedB->imm / 4), static_cast<unsigned long>(1));
            if (offset >= textInstructions.size()) continue;
            if (castedB->imm < 0) offset--;

            // If there's already an entry point at the end of the jump
            if (auto* castedE =
                    dynamic_cast<EntryPoint*>(textInstructions[offset].get())) {
                textInstructions[i] = move(
                    make_unique<BInstructionEntry>(castedB, castedE->name));
            } else if (auto it = tmps.find(offset);
                       it != tmps.end()) {  // If a temp entry point has
                                            // already been added
                textInstructions[i] =
                    move(make_unique<BInstructionEntry>(castedB, it->second));
            } else {  // Otherwise add a new entry points

                // Check if there's already a symbol with the same name as the
                // new temp entry point (Unlikely to trigger)
                while (asmFile->getSymbolName(
                           "L.diss." + std::to_string(tmpCount)) != nullopt)
                    tmpCount++;  // Unlikely to trigger

                // Add a new entry point
                tmps.insert({offset, "L.diss." + std::to_string(tmpCount)});
                entryPoints.push_back(
                    make_pair("L.diss." + std::to_string(tmpCount),
                              Assembly::SymbolBinding::LOCAL));
                textInstructions[i] = move(make_unique<BInstructionEntry>(
                    castedB, "L.diss." + std::to_string(tmpCount)));
                tmpCount++;
            }
        }
    }

    // New entry points are not inserted into the instructions until the end so
    // explicit jumps can still accurately calculate difference to existing
    // entry points
    for (auto tmp : tmps) {
        textInstructions.insert(textInstructions.begin() + tmp.first,
                                make_unique<EntryPoint>(tmp.second));
    }

    size_t i = 0;
    // Translate unravelled pseudo-intructions
    while (i < textInstructions.size()) {
        auto& instr = textInstructions[i];
        uint32_t symbolAddress;
        vector<Disassembler::Symbol> vars;

        // If it uses a gp relative address, convert from that to the symbol
        // name
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

        // If it uses pc-relative address, convert from that to the symbol
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

    return make_shared<TextSection>(move(textInstructions), entryPoints);
}

unique_ptr<RiscvFile> disassemble(const string& filepath) {
    // Parse the binary file
    unique_ptr<ELFParser::ELFFile> elffile = ELFParser::parseFile(filepath);
    if (!elffile) return nullptr;

    auto asmFile = make_unique<RiscvFile>(
        static_cast<FileType>(elffile->getHeader()->fileType));

    const unordered_map<string, unique_ptr<ELFParser::ELFSection>>& sections =
        elffile->getSections();

    // Decode symbol table
    disassembleSymbolTable(asmFile, elffile, sections);

    // Decode the .data section if present and non-empty
    if (auto dataIt = sections.find(".data");
        dataIt != sections.end() && dataIt->second->header->size != 0) {
        auto dataSection = disassembleDataSection(asmFile, (*dataIt).second,
                                                  elffile->isLittleEndian);
        if (!dataSection->empty()) asmFile->addSection(".data", dataSection);
    }

    // Decode the .bss section if present and non-empty
    if (auto bssIt = sections.find(".bss"); bssIt != sections.end()) {
        auto bssSection = disassembleBSSSection(asmFile, (*bssIt).second);
        if (!bssSection->empty()) asmFile->addSection(".bss", bssSection);
    }

    auto textIt = sections.find(".text");

    // Decode .text section
    if (textIt == sections.end()) {
        throw ELFParser::BadFileException("No text section found.");
    }

    auto gpSym = asmFile->getSymbolName("__global_pointer$");
    uint32_t gpAddress = gpSym.has_value() ? gpSym.value().get().addr : 0;

    auto textSection = disassembleTextSection(
        asmFile, (*textIt).second, gpAddress, elffile->isLittleEndian);

    asmFile->addSection(".text", move(textSection));

    return asmFile;
}
}  // namespace Disassembler
