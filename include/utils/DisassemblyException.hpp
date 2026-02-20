#ifndef DISASSEMBLYEXCEPTION_H
#define DISASSEMBLYEXCEPTION_H

#include <stdexcept>
#include <string>

namespace Disassembler {

/**
 * Represents an exception that occurs during the disassembly
 * Can indicate invalid data from the file, missing .text section, etc
 */
class DisassemblyException : public std::runtime_error {
   public:
    DisassemblyException()
        : runtime_error("An exception occurred during disassembly.") {};
    DisassemblyException(std::string err) : runtime_error(err.c_str()) {};
};
}  // namespace Disassembler

#endif