#include <memory>

#include "utils/AssemblyFile.hpp"

std::unique_ptr<AssemblyFile> disassemble(const std::string& filepath);