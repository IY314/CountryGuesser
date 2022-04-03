#include <regex>

#include "csv.hh"

std::vector<std::vector<std::string>> csv::readCSV(std::ifstream& is) {
    // CSV data regex that detects any non-delimiter (newline and comma)
    // sequence
    std::regex re{R"(([^,\r\n]+))"};

    std::vector<std::vector<std::string>> output;

    // Iterate through every line
    for (std::string line{}; std::getline(is, line);)
        // Use a regex iterator to push all matches to output
        output.emplace_back(std::vector<std::string>(
            std::sregex_token_iterator(line.begin(), line.end(), re, 1), {}));

    return output;
}
