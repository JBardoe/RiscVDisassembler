#include "disassembler/Disassembler.hpp"

#include <cstdint>
#include <format>
#include <iostream>
#include <vector>

#include "disassembler/ELFFile.hpp"
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

    asmFile->addSection(make_unique<TextSection>(move(textInstructions)));

    std::cout << "Sections:\n";

    for (auto& sec : sections) {
        std::cout << "Name: " << sec.first
                  << ". Offset: " << sec.second->header->offset << "\n";
    }

    return asmFile;
}
}  // namespace Disassembler