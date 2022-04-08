// guess.cc
// Author: Isaac Yee
// Game logic implementation

#include <regex>

#include "game.hh"
#include "util.hh"

cg::Equals::Equals(const std::string& a) : a{a} {}

bool cg::Equals::operator()(const std::string& b) const {
    return util::levenshteinDist(a, b) <= 2;
}

cg::Replacer::Replacer(const std::string& src) : src{src} {}

cg::Replacer& cg::Replacer::replaceAll() {
    return replaceAmpersand().replaceSaint().replaceCardinalAbbrev();
}

cg::Replacer& cg::Replacer::replaceAmpersand() {
    // Matches &
    src = std::regex_replace(src, std::regex("&"), "and");

    return *this;
}

cg::Replacer& cg::Replacer::replaceSaint() {
    // Matches St. <something> or St <something>
    src = std::regex_replace(
        src, std::regex(R"(^st\.?\s)", std::regex_constants::icase), "Saint ");

    return *this;
}

cg::Replacer& cg::Replacer::replaceCardinalAbbrev() {
    // Matches N. <something> or N <something>
    src = std::regex_replace(
        src, std::regex(R"(^n\.?\s)", std::regex_constants::icase), "North ");
    // Matches S. <something> or S <something>
    src = std::regex_replace(
        src, std::regex(R"(^s\.?\s)", std::regex_constants::icase), "South ");
    // Matches E. <something> or E <something>
    src = std::regex_replace(
        src, std::regex(R"(^e\.?\s)", std::regex_constants::icase), "East ");
    // Matches W. <something> or W <something>
    src = std::regex_replace(
        src, std::regex(R"(^w\.?\s)", std::regex_constants::icase), "West ");

    return *this;
}

cg::Replacer& cg::Replacer::set(std::string& dest) {
    dest = src;
    return *this;
}

void cg::Game::processGuess() {
    // Various easter eggs :)
    Equals inputEquals(guess);
    std::vector<std::string>::const_iterator it;

    if (std::any_of(easterEggs.begin(), easterEggs.end(),
                    inputEquals)) {  // If the player types in anything that
                                     // makes them a wisebutt
        popup("Nice one there, wisebutt.");
    } else if ((it = std::find_if(metaEasterEggs.begin(), metaEasterEggs.end(),
                                  inputEquals)) !=
               metaEasterEggs.end()) {       // If the player then proceeds to
                                             // type in the game's response
        if (*it == metaEasterEggs.back()) {  // If the player has reached
                                             // the end of the list
            lose("I warned you, didn't I? YOU LOSE!");
        } else {  // Display the *next* element in the list
            popup(*(++it));
        }
    } else if (inputEquals("You Lose!")) {
        lose("No, YOU lose!");
    } else {
        validateGuess();
    }
}

void cg::Game::validateGuess() {
    size_t lowestDistance = 3;
    size_t idx = 0;
    // Iterate through all countries
    for (auto i = countries.begin(); i != countries.end(); ++i)
        // Some countries have multiple names, so each country must also be
        // iterated through
        for (auto ci = (*i).begin(); ci != (*i).end(); ++ci) {
            // Fuzzy check of guess and country name
            size_t d = util::levenshteinDist(guess, *ci);

            // Check that the guess and country is closer than previous
            if (d < lowestDistance) {
                lowestDistance = d;
                idx = i - countries.begin();
                if (d == 0) {
                    // There will only be one country with a distance of 0, so
                    // return early
                    if (std::find(guessed.begin(), guessed.end(), idx) !=
                        guessed.end()) {
                        popup("That country has already been guessed!");
                    } else {
                        guessed.push_back(idx);
                        if (guessed.size() == required) win();
                    }
                    return;
                }
            }
        }

    // 3 is the upper bound, meaning that if no countries were found that were
    // closer than 3 edits, the guess is not valid
    if (lowestDistance == 3) {
        lose("You Lose!");
    } else {
        // Use closest country
        if (std::find(guessed.begin(), guessed.end(), idx) != guessed.end()) {
            popup("That country has already been guessed!");
        } else {
            guessed.push_back(idx);
            if (guessed.size() == required) win();
        }
    }
}
