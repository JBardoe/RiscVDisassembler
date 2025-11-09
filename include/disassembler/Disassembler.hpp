#include <elfio/elfio.hpp>
#include <optional>

#include "utils/AssemblyFile.hpp"

std::unique_ptr<AssemblyFile> disassemble(const std::string& filepath);