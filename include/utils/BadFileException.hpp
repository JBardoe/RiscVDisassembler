#include <stdexcept>

namespace ELFParser {

class BadFileException : public std::runtime_error {
   public:
    BadFileException() : runtime_error("File is not properly formatted.") {};
    BadFileException(std::string err) : runtime_error(err.c_str()) {};
};
}  // namespace ELFParser