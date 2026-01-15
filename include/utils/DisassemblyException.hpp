#ifndef DISASSEMBLYEXCEPTION_H
#define DISASSEMBLYEXCEPTION_H

#include <stdexcept>

namespace Disassembler {

class DisassemblyException : public std::runtime_error {
   public:
    DisassemblyException()
        : runtime_error("An exception occurred during disassembly.") {};
    DisassemblyException(std::string err) : runtime_error(err.c_str()) {};
};
}  // namespace Disassembler

#endif