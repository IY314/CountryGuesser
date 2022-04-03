#include "util.hh"

std::string util::trim(const std::string& str, const std::string& whitespace) {
    const auto strBegin = str.find_first_not_of(whitespace);
    if (strBegin == std::string::npos) return "";  // no content

    const auto strEnd = str.find_last_not_of(whitespace);
    const auto strRange = strEnd - strBegin + 1;

    return str.substr(strBegin, strRange);
}

std::string util::reduce(const std::string& str, const std::string& fill,
                         const std::string& whitespace) {
    // trim first
    auto result = trim(str, whitespace);

    // replace sub ranges
    auto beginSpace = result.find_first_of(whitespace);
    while (beginSpace != std::string::npos) {
        const auto endSpace = result.find_first_not_of(whitespace, beginSpace);
        const auto range = endSpace - beginSpace;

        result.replace(beginSpace, range, fill);

        const auto newStart = beginSpace + fill.length();
        beginSpace = result.find_first_of(whitespace, newStart);
    }

    return result;
}

size_t util::levenshteinDist(const std::string& s1, const std::string& s2) {
    const size_t m(s1.size()), n(s2.size());

    if (m == 0) return n;
    if (n == 0) return m;

    // allocation below is not ISO-compliant,
    // it won't work with -pedantic-errors.
    size_t costs[n + 1];

    for (size_t k = 0; k <= n; k++) costs[k] = k;

    size_t i{0};
    for (char const& c1 : s1) {
        costs[0] = i + 1;
        size_t corner{i}, j{0};
        for (char const& c2 : s2) {
            size_t upper{costs[j + 1]};
            if (std::tolower(c1) == std::tolower(c2))
                costs[j + 1] = corner;
            else {
                size_t t(upper < corner ? upper : corner);
                costs[j + 1] = (costs[j] < t ? costs[j] : t) + 1;
            }

            corner = upper;
            j++;
        }
        i++;
    }

    return costs[n];
}
