#pragma once

#include <fstream>
#include <string>
#include <vector>

namespace csv {
using CSVRow = std::vector<std::string>;
using CSVOutput = std::vector<CSVRow>;

CSVOutput readCSV(std::ifstream& is);
}  // namespace csv
