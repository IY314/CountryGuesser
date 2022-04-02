#include "game.hh"
#include "guess.hh"
#include "util.hh"

cg::caseInsensitiveEquals::caseInsensitiveEquals(const std::string& a) : a{a} {}

bool cg::caseInsensitiveEquals::operator()(const std::string& b) const {
    return std::equal(
        a.begin(), a.end(), b.begin(), b.end(),
        [](char x, char y) { return std::tolower(x) == std::tolower(y); });
}

void cg::replaceAll(std::string& src) {
    cg::replaceAmpersand(src);
    cg::replaceSaint(src);
}

void cg::replaceAmpersand(std::string& src) {
    size_t i = 0;
    while (i < (src.size() - 1)) {
        if (src.at(i) == '&') {
            src.insert(i + 1, "and");
            src.erase(i, 1);
            i += 3;
        } else
            ++i;
    }
}

void cg::replaceSaint(std::string& src) {
    if (src.size() < 2) return;
    const cg::caseInsensitiveEquals isSt("st");
    size_t i = 0;
    bool inserted = false;
    while (i < (src.size() - 2)) {
        if (isSt(src.substr(i, 2))) {
            src.insert(i + 1, "ain");
            inserted = true;
            i += 5;
        } else if (inserted && src.at(i) == '.') {
            src.erase(i, 1);
            inserted = false;
        } else
            ++i;
    }
}

void cg::Game::processGuess() {
    // Various easter eggs :)
    caseInsensitiveEquals inputEquals(guess);
    if (std::any_of(easterEggs.begin(), easterEggs.end(), inputEquals)) {
        popup("Nice one there, wisebutt.");
    } else if (std::any_of(metaEasterEggs.begin(), metaEasterEggs.end(),
                           inputEquals)) {
        for (auto it = metaEasterEggs.begin(); it != metaEasterEggs.end(); ++it)
            if (inputEquals(*it)) {
                if (it == metaEasterEggs.end() - 1) {
                    lose("I warned you, didn't I? YOU LOSE!");
                } else {
                    popup(metaEasterEggs.at(it - metaEasterEggs.begin() + 1));
                }
            }
    } else if (inputEquals("You Lose!")) {
        lose("No, YOU lose!");
    }

    // Check validity of guess
    validateGuess(inputEquals);
}

void cg::Game::validateGuess(const caseInsensitiveEquals& inputEquals) {
    // Iterate through all countries
    for (auto i = countries.begin(); i != countries.end(); ++i) {
        // Some countries have multiple names, so each country must also be
        // iterated through
        for (auto ci = (*i).begin(); ci != (*i).end(); ++ci) {
            // Case-insensitive check of guess and country name
            if (inputEquals(*ci)) {
                unsigned long idx = i - countries.begin();
                if (std::find(guessed.begin(), guessed.end(), idx) !=
                    guessed.end()) {
                    popup("That country has already been guessed!");
                } else {
                    guessed.push_back(idx);
                }
                return;
            }
        }
    }

    lose("You Lose!");
}
