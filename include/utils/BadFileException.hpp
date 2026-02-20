#ifndef BADFILEEXCEPTION_H
#define BADFILEEXCEPTION_H

#include <stdexcept>
#include <string>

namespace ELFParser {
/**
 * Represents an exception caused by the ELF parser
 * Indicates a problem with the ELF file such as due to corruption or incomplete
 * data
 */
class BadFileException : public std::runtime_error {
   public:
    BadFileException() : runtime_error("File is not properly formatted.") {};
    BadFileException(std::string err) : runtime_error(err.c_str()) {};
};
}  // namespace ELFParser

#endif