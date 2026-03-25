#ifndef ASSEMBLYTYPES_HPP
#define ASSEMBLYTYPES_HPP

#include <array>
#include <cstdint>
#include <string>

namespace Assembly {

/**
 * Struct representing a variable in the data sections
 */
typedef struct Variable {
    std::string name;
    uint32_t addr;
    uint32_t val;
    uint32_t size;
} Variable;

/**
 * toString for the variable struct
 *
 * @param v variable struct
 * @return string equivalent
 */
std::string to_string(Variable v);

/**
 * Lookup array to match variable size to the string name of type
 */
static constexpr std::array<std::string_view, 4> variableTypes{
    ".byte", ".half", "<unknown_type>", ".word"};

/**
 * Formatting constant to position comments away from instructions in assembly
 * output
 */
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

enum class BranchDirection : int {
    FORWARD,
    BACKWARD,
    JUMP,
    NA,
};

}  // namespace Assembly
#endif