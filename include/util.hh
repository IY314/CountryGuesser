#pragma once

#include <string>

namespace util {
std::string trim(const std::string& str, const std::string& whitespace = " \t");

std::string reduce(const std::string& str, const std::string& fill = " ",
                   const std::string& whitespace = " \t");
}  // namespace util
