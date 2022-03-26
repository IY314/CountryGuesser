#include <regex>

#include "csv.hh"

csv::CSVOutput csv::readCSV(std::ifstream& is) {
    std::regex re{R"(([^,\r\n]+))"};

    csv::CSVOutput output;

    for (std::string line{}; std::getline(is, line);)
        output.emplace_back(csv::CSVRow(
            std::sregex_token_iterator(line.begin(), line.end(), re, 1), {}));

    return output;
}
