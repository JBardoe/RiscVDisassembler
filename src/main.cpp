#include <stdlib.h>

#include <cstdint>
#include <iomanip>
#include <iostream>
#include <string>

#include "analyser/Analyser.hpp"
#include "analyser/AnalysisTypes.hpp"
#include "disassembler/Disassembler.hpp"
#include "translator/ArmFile.hpp"
#include "utils/AnalysisException.hpp"
#include "utils/BadFileException.hpp"
#include "utils/DisassemblyException.hpp"
#include "utils/TranslationException.hpp"

int main() {
    while (true) {
        std::string fileName = "";

        std::cout << "Enter file name: ";
        std::cin >> fileName;

        if (fileName == "q" || fileName == "") break;

        system("clear");

        std::unique_ptr<Disassembler::RiscvFile> riscFile;

        try {
            riscFile =
                Disassembler::disassemble("data/test/elf/" + fileName + ".elf");
        } catch (const ELFParser::BadFileException& e) {
            std::cout << "An Error Occurred During Parsing:\n"
                      << e.what() << "\n";
            return 1;
        } catch (const Disassembler::DisassemblyException& e) {
            std::cout << "An Error Occurred During Disassembly:\n"
                      << e.what() << "\n";
            return 1;
        }

        std::cout << "<<<<<<<<<<<<<<<<RISC-V File>>>>>>>>>>>>>>>>\n";

        std::cout << riscFile->toString();

        Translator::ArmFile armFile;

        try {
            armFile = Translator::ArmFile(riscFile);
        } catch (const Translator::TranslationException& e) {
            std::cout << "An Error Occurred During Translation:\n"
                      << e.what() << "\n";
            return 1;
        }

        std::cout
            << "<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>\n\n<<<<<<<<<<<<<<<<"
               "AArch64 File>>>>>>>>>>>>>>>>\n";

        std::cout << armFile.toString();

        std::cout << "<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>\n";

        riscFile->writeToFile(fileName);
        armFile.writeToFile(fileName);

        try {
            Analyser::analyse(armFile);
        } catch (const Analyser::AnalysisException& e) {
            std::cout << "An Error Occurred During Analysis:\n"
                      << e.what() << "\n";
            return 1;
        }

        auto& analysisReport = armFile.getAnalysis();

        std::cout << "\n<<<<<<<<<<<<<<<<"
                     "Analysis>>>>>>>>>>>>>>>>\n"
                  << Analyser::to_string(*analysisReport)
                  << "<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>\n";
    }

    return 0;
}