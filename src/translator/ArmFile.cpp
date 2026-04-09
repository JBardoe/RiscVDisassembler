#include "translator/ArmFile.hpp"

#include <algorithm>
#include <array>
#include <fstream>
#include <ranges>
#include <vector>

#include "disassembler/RiscvSection.hpp"

namespace Translator {

/**
 * Replaces all references to a register with another in a given section
 * @param older the register to replace
 * @param newer the replacement register
 * @param instructions the instructions to replace over
 * @param start start index of the section in which to replace
 * @param end end index of the section in which to replace
 *
 * @return whether the replaced register was ever written to
 */
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

/**
 * Eliminates a register from a .text section
 * @param reg the register to eliminate
 * @param riscTextSection the .text section to eliminate from (in RISC-V)
 *
 * @return a map of the instructions to insert to allow for the elimination in
 * the form (index to insert before, instructions to insert)
 */
std::unordered_map<int, std::vector<std::unique_ptr<ArmInstruction>>>
eliminateRegister(int reg, Disassembler::TextSection* riscTextSection) {
    // Names of the variables inserted to allow for spilling
    std::string regVarName =
        Disassembler::to_string(static_cast<Disassembler::Register>(
            reg));  // Spilled value of the register
    std::string regSpillName =
        regVarName + "Spill";  // Spilled value of the register the previous
                               // variable is loaded into

    std::unordered_map<int, std::vector<std::unique_ptr<ArmInstruction>>> ret;

    std::vector<std::unique_ptr<Disassembler::RiscvInstruction>>&
        riscTextInstructions = riscTextSection->getInstructions();
    auto registersUsed = riscTextSection->getRegistersUsed();
    auto basicBlocks = riscTextSection->getBasicBlocks();

    // Check if the register can be replaced globally with another free register
    for (auto& tempReg : Disassembler::tempRegisters) {
        if (!registersUsed->count(static_cast<int>(tempReg))) {
            replaceRegister(reg, static_cast<int>(tempReg),
                            riscTextInstructions, 0,
                            riscTextInstructions.size() - 1);
            registersUsed->erase(reg);
            ret[1].push_back(std::make_unique<RSInstruction>(
                Operator::ldrsw, static_cast<Register>(tempReg), regVarName));
            return ret;
        }
    }

    // Otherwise, replace locally
    for (auto block = basicBlocks->begin(); block != basicBlocks->end();
         block++) {
        if (block->first == -1) continue;  // Skip the empty first block
        if (!block->second->count(reg))
            continue;  // Skip blocks that don't use the register

        // Check if any temp register is not used in this block and use it for
        // the eliminated register if so
        for (auto& tempReg : Disassembler::tempRegisters) {
            if (!block->second->count(static_cast<int>(tempReg))) {
                bool written = replaceRegister(
                    reg, static_cast<int>(tempReg), riscTextInstructions,
                    std::prev(block)->first + 1, block->first);
                block->second->erase(reg);

                ret[std::prev(block)->first + 1].push_back(
                    std::make_unique<RSInstruction>(
                        Operator::ldrsw, static_cast<Register>(tempReg),
                        regVarName));

                if (written) {
                    ret[block->first].push_back(std::make_unique<RSInstruction>(
                        Operator::str, static_cast<Register>(tempReg),
                        regVarName));
                }
                return ret;
            }
        }

        int i =
            std::prev(block)->first + 1;  // Base instruction of the register

        // Replace locally within the block
        // NOTE performance inefficient but much better resulting code
        while (i <= block->first) {
            // Find the next instruction using the register to be eliminated
            while (i <= block->first &&
                   riscTextInstructions[i]->usesRegister(
                       static_cast<Disassembler::Register>(reg)) == 0) {
                i++;
            }

            if (i > block->first) break;  // Should not trigger

            std::array<int, 7> nextTmpUse{
                -1, -1, -1, -1,
                -1, -1, -1};  // Index of the next time each temp instruction is
                              // used
            int count = 0;
            int j = i;

            // Iterate over all the remaining instructions or until all temps
            // have been used and mark the index of their next use
            while (j <= block->first && count < 7) {
                for (auto& usedReg :
                     riscTextInstructions[j]->getRegistersUsed()) {
                    if (Disassembler::tempRegisters.count(usedReg)) {
                        int index =
                            static_cast<int>(usedReg) -
                            ((usedReg < Disassembler::Register::t3) ? 5 : 25);

                        if (nextTmpUse[index] == -1) {
                            nextTmpUse[index] = j;
                            count++;
                        }
                    }
                }
                j++;
            }

            // Sort the indexes to find the last used temp
            std::array<int, 7> indexes = {0, 1, 2, 3, 4, 5, 6};

            std::sort(indexes.begin(), indexes.end(), [&](int a, int b) {
                return nextTmpUse[a] < nextTmpUse[b];
            });

            bool written = false;
            int tempReg = -1;

            // If there is a temp that is never used again, it can be used for
            // the eliminated register
            if (nextTmpUse[indexes.front()] == -1) {
                tempReg = indexes.front() + ((indexes.front() < 3) ? 5 : 25);
                written = replaceRegister(reg, tempReg, riscTextInstructions, i,
                                          block->first);
                j = block->first;
            } else {  // Otherwise use the last used temp
                tempReg = indexes.back() + ((indexes.back() < 3) ? 5 : 25);
                written = replaceRegister(reg, tempReg, riscTextInstructions, i,
                                          nextTmpUse[indexes.back()] - 1);
                j = nextTmpUse[indexes.back()];
            }

            // Add the code required to load and store the register
            ret[i].push_back(std::make_unique<RSInstruction>(
                Operator::str, static_cast<Register>(tempReg), regSpillName));

            ret[i].push_back(std::make_unique<RSInstruction>(
                Operator::ldrsw, static_cast<Register>(tempReg), regVarName));

            if (written) {
                ret[j].push_back(std::make_unique<RSInstruction>(
                    Operator::str, static_cast<Register>(tempReg), regVarName));
            }

            if (j == nextTmpUse[indexes.back()]) {
                ret[j].push_back(std::make_unique<RSInstruction>(
                    Operator::ldrsw, static_cast<Register>(tempReg),
                    regSpillName));
            }

            i = j;
        }
    }
    return ret;
}

ArmFile::ArmFile(const std::unique_ptr<Disassembler::RiscvFile>& riscFile)
    : printOut("") {
    auto& riscSections = riscFile->getSections();
    auto* riscTextSection = dynamic_cast<Disassembler::TextSection*>(
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

    // Whether gp and tp need to be spilled
    bool gpSpill = false;
    bool tpSpill = false;

    std::unordered_map<int, std::vector<std::unique_ptr<ArmInstruction>>> toAdd;

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

    // If gp or tp need to be spilled, add variables to do that
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
            auto gpSym = riscFile->getSymbolName(
                "__global_pointer$");  // Get gp initial value
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

    // Add in the the instructions needed to spill the registers
    for (auto& addition : toAdd) {
        int i = addition.first;

        while (instructions[i][0]->instr == Operator::entry) {
            i++;
        }

        instructions[i].insert(instructions[i].begin(),
                               std::make_move_iterator(addition.second.begin()),
                               std::make_move_iterator(addition.second.end()));
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
