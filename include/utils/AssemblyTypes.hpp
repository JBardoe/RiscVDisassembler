#ifndef ASSEMBLYTYPES_HPP
#define ASSEMBLYTYPES_HPP

#include <array>
#include <cstdint>
#include <string>

namespace Assembly {

typedef struct Variable {
    std::string name;
    uint32_t addr;
    uint32_t val;
    uint32_t size;
} Variable;

std::string to_string(Variable v);

static constexpr std::array<std::string_view, 4> variableTypes{
    ".byte", ".half", "<unknown_type>", ".word"};

static constexpr std::size_t COMMENT_COL = 25;

/**
 * All possible bindings for entries in the symbol table
 * Underlying value is the equivalent constant value that represents the binding
 * in the file
 */
enum class SymbolBinding : int {
    LOCAL = 0,
    GLOBAL = 1,
    WEAK = 2,
    LOOS = 10,
    HIOS = 12,
    LOPROC = 13,
    HIPROC = 15,
};

/**
 * toString for the symbol binding enum
 *
 * @param b symbol binding enum
 * @return string equivalent
 */
std::string to_string(SymbolBinding b);

}  // namespace Assembly
#endif