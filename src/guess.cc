#include "game.hh"
#include "guess.hh"
#include "util.hh"

cg::guess::caseInsensitiveEquals::caseInsensitiveEquals(const std::string& a)
    : a{a} {}

bool cg::guess::caseInsensitiveEquals::operator()(const std::string& b) const {
    return std::equal(
        a.begin(), a.end(), b.begin(), b.end(),
        [](char x, char y) { return std::tolower(x) == std::tolower(y); });
}

void cg::guess::replaceAll(std::string& src) {
    cg::guess::replaceAmpersand(src);
    cg::guess::replaceSaint(src);
}

void cg::guess::replaceAmpersand(std::string& src) {
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

void cg::guess::replaceSaint(std::string& src) {
    if (src.size() < 2) return;
    const cg::guess::caseInsensitiveEquals isSt("st");
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
    cg::guess::caseInsensitiveEquals inputEquals(input);
    if (std::any_of(cg::guess::easterEggs.begin(), cg::guess::easterEggs.end(),
                    inputEquals)) {
        msg = "Nice one there, wisebutt.";
        status = gs_note;
    } else if (std::any_of(cg::guess::metaEasterEggs.begin(),
                           cg::guess::metaEasterEggs.end(), inputEquals)) {
        for (auto it = cg::guess::metaEasterEggs.begin();
             it != cg::guess::metaEasterEggs.end(); ++it)
            if (inputEquals(*it)) {
                if (it == cg::guess::metaEasterEggs.end() - 1) {
                    msg = "I warned you, didn't I? YOU LOSE!";
                    status = gs_lose;
                } else {
                    msg = cg::guess::metaEasterEggs.at(
                        it - cg::guess::metaEasterEggs.begin() + 1);
                    status = gs_note;
                }
            }
    } else if (inputEquals("You Lose!")) {
        msg = "No, YOU lose!";
        status = gs_lose;
    }

    // Check validity of guess
    validateGuess(inputEquals);
}

void cg::Game::validateGuess(const guess::caseInsensitiveEquals& inputEquals) {
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
                    msg = "That country has already been guessed!";
                    status = gs_note;
                } else {
                    msg = std::to_string(idx);
                    status = gs_ok;
                }
                return;
            }
        }
    }

    msg = "You Lose!";
    status = gs_lose;
}
