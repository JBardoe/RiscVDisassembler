#include <elfio/elfio.hpp>
#include <optional>

#include "utils/AssemblyFile.hpp"

std::optional<ELFIO::elfio> get_file(const std::string& filepath);
std::optional<AssemblyFile> disassemble(const std::string& filepath);