#ifndef ANALYSISEXCEPTION_H
#define ANALYSISEXCEPTION_H

#include <stdexcept>
#include <string>

namespace Analyser {

/**
 * Represents an exception that occurs during the disassembly
 * Can indicate invalid data from the file, missing .text section, etc
 */
class AnalysisException : public std::runtime_error {
   public:
    AnalysisException()
        : runtime_error("An exception occurred during analysis.") {};
    AnalysisException(std::string err) : runtime_error(err.c_str()) {};
};
}  // namespace Analyser

#endif