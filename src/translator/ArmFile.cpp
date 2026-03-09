#include "translator/ArmFile.hpp"

#include <fstream>
#include <vector>

#include "disassembler/RiscvSection.hpp"

namespace Translator {

bool replaceRegister(
    int older, int newer,
    std::vector<std::unique_ptr<Disassembler::RiscvInstruction>>& instructions,
    std::size_t start, std::size_t end) {
    bool writesToOld = false;
    for (size_t i = start; i <= end; i++) {
        if (instructions[i]->usesRegister(
                static_cast<Disassembler::Register>(older)) == 2)
            writesToOld = true;
        instructions[i]->replaceRegister(
            static_cast<Disassembler::Register>(older),
            static_cast<Disassembler::Register>(newer));
    }

    return writesToOld;
}

std::map<int, std::vector<std::unique_ptr<ArmInstruction>>> eliminateRegister(
    int reg, Disassembler::TextSection* riscTextSection) {
    std::string regVarName =
        Disassembler::to_string(static_cast<Disassembler::Register>(reg));
    std::string regSpillName = regVarName + "Spill";

    std::map<int, std::vector<std::unique_ptr<ArmInstruction>>> ret;

    std::vector<std::unique_ptr<Disassembler::RiscvInstruction>>&
        riscTextInstructions = riscTextSection->getInstructions();
    auto registersUsed = riscTextSection->getRegistersUsed();
    auto basicBlocks = riscTextSection->getBasicBlocks();

    // If can be replaced globally
    for (auto& tempReg : Disassembler::tempRegisters) {
        if (!registersUsed->count(static_cast<int>(tempReg))) {
            replaceRegister(reg, static_cast<int>(tempReg),
                            riscTextInstructions, 0,
                            riscTextInstructions.size() - 1);
            registersUsed->erase(reg);
            ret[1].push_back(std::make_unique<RSInstruction>(
                Operator::ldr, static_cast<Register>(tempReg), regVarName));
            return ret;
        }
    }

    // Replace locally
    for (auto block = basicBlocks->begin(); block != basicBlocks->end();
         block++) {
        if (block->first == -1) continue;
        if (!block->second->count(reg)) continue;

        for (auto& tempReg : Disassembler::tempRegisters) {
            if (!block->second->count(static_cast<int>(tempReg))) {
                bool written = replaceRegister(
                    reg, static_cast<int>(tempReg), riscTextInstructions,
                    std::prev(block)->first + 1, block->first);
                block->second->erase(reg);

                ret[std::prev(block)->first + 1].push_back(
                    std::make_unique<RSInstruction>(
                        Operator::ldr, static_cast<Register>(tempReg),
                        regVarName));

                if (written) {
                    ret[block->first].push_back(std::make_unique<RSInstruction>(
                        Operator::str, static_cast<Register>(tempReg),
                        regVarName));
                }
                return std::move(ret);
            }
        }
        // TODO Eliminate Register From Block
    }
}

ArmFile::ArmFile(const std::unique_ptr<Disassembler::RiscvFile>& riscFile)
    : printOut("") {
    auto riscSections = riscFile->getSections();
    auto riscTextSection = dynamic_cast<Disassembler::TextSection*>(
        riscSections.find(".text")->second.get());
    std::vector<std::unique_ptr<Disassembler::RiscvInstruction>>&
        riscTextInstructions = riscTextSection->getInstructions();

    auto registersUsed = riscTextSection->getRegistersUsed();

    // Transfer .data and .bss over directly if present (no changes are
    // required)
    if (auto it = riscSections.find(".data"); it != riscSections.end()) {
        sections.insert({".data", it->second});
    }

    if (auto it = riscSections.find(".bss"); it != riscSections.end()) {
        sections.insert({".bss", it->second});
    }

    bool gpSpill = false;
    bool tpSpill = false;

    std::map<int, std::vector<std::unique_ptr<ArmInstruction>>> toAdd;

    // Eliminate tp + gp
    if (registersUsed->count(3)) {
        gpSpill = true;
        toAdd = std::move(eliminateRegister(3, riscTextSection));
    }

    if (registersUsed->count(4)) {
        tpSpill = true;
        auto temp = eliminateRegister(4, riscTextSection);

        for (auto& addition : temp) {
            if (auto it = toAdd.find(addition.first); it != toAdd.end()) {
                it->second.insert(
                    it->second.end(),
                    std::make_move_iterator(addition.second.begin()),
                    std::make_move_iterator(addition.second.end()));
            } else {
                toAdd.insert(std::move(addition));
            }
        }
    }

    // TODO add toAdd to text instruction - if at entrypoint, add after

    if (gpSpill || tpSpill) {
        auto dataIt = sections.find(".data");
        if (dataIt == sections.end()) {
            sections.insert(
                {".data", std::make_shared<Assembly::DataSection>()});
            dataIt = sections.find(".data");
        }

        auto* dataSection =
            dynamic_cast<Assembly::DataSection*>(dataIt->second.get());

        if (gpSpill) {
            auto gpSym = riscFile->getSymbolName("__global_pointer$");
            dataSection->addVariable(
                "gp", (gpSym.has_value()) ? gpSym.value().get().addr : 0, 4);
            dataSection->addVariable("gpSpill", 0, 4);
        }
        if (tpSpill) {
            dataSection->addVariable("tp", 0, 4);
            dataSection->addVariable("tpSpill", 0, 4);
        }
    }

    // Translate text instructions into ARM
    std::vector<std::vector<std::unique_ptr<ArmInstruction>>>
        instructions;  // 2D as some instructions expand into two

    for (auto& instr : riscTextInstructions) {
        instructions.push_back(std::move(instr->toArm()));
    }

    sections.insert(
        {".text",
         std::make_shared<TextSection>(
             std::move(instructions), riscTextSection->getEntryPoints(),
             riscTextSection->getBasicBlocks(), registersUsed)});
}

const std::string& ArmFile::toString() {
    if (this->printOut != "") return this->printOut;

    for (auto& sec : this->sections) {
        printOut += sec.second->toString();
        printOut += "\n";
    }

    return this->printOut;
}

void ArmFile::writeToFile(std::string fileName) {
    std::ofstream outFile("data/output/ARM/" + fileName + ".s");

    outFile << toString();

    outFile.close();
}

}  // namespace Translator
