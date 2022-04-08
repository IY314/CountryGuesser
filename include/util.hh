// util.hh
// Author: Isaac Yee
// Utility function declarations

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

// Code I got from the Internet (NOT TOUCHING THAT EVER) (as long as it works)
size_t levenshteinDist(const std::string& text, const std::string& pattern);
}  // namespace util
