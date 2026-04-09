#ifndef TRANSLATIONEXCEPTION_H
#define TRANSLATIONEXCEPTION_H

#include <stdexcept>
#include <string>

namespace Translator {
/**
 * Represents an exception caused by the translator
 * Indicates a problem with the RISC-V file
 */
class TranslationException : public std::runtime_error {
   public:
    TranslationException()
        : runtime_error("An exception occurred during translation.") {};
    TranslationException(std::string err) : runtime_error(err.c_str()) {};
};
}  // namespace Translator

#endif