// csv.hh
// Author: Isaac Yee
// CSV file parsing declaration

#pragma once

#include <fstream>
#include <string>
#include <vector>

namespace csv {
// Read CSV from a file and output a vector
std::vector<std::vector<std::string>> readCSV(std::ifstream& is);
}  // namespace csv
