#pragma once

#include <string>

#define util__addcolor(c, t, h)   \
    if (h) attron(COLOR_PAIR(c)); \
    t;                            \
    if (h) attroff(COLOR_PAIR(c))

namespace util {
std::string trim(const std::string& str, const std::string& whitespace = " \t");

std::string reduce(const std::string& str, const std::string& fill = " ",
                   const std::string& whitespace = " \t");
}  // namespace util
