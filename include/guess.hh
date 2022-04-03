#pragma once

#include "csv.hh"

namespace cg {
// Easter egg values
const std::vector<std::string> easterEggs{
    "That country has already been guessed!",
    "A country",
    "This country",
    "That country",
    "Enter a country:",
    "Enter a country"};

// Meta easter eggs, in increasing agitation
const std::vector<std::string> metaEasterEggs{
    "Nice one there, wisebutt.",
    "You think you're funny?",
    "Ok, this is getting out of hand.",
    "Are you actually trying to play the game?",
    "Ok, I'm warning you. DO NOT TYPE THIS IN.",
    "Why'd you type it in?",
    "I'm done with you. If you type this in, you die."};

// Equality functor
class Equals {
    std::string a;

   public:
    // Initialize a
    Equals(const std::string& a);

    // Compare
    bool operator()(const std::string& b) const;
};

// String replacer
class Replacer {
   public:
    std::string src;

    // Init replacer
    Replacer(const std::string& src);

    // Replace all matches in a string
    Replacer& replaceAll();

    // Expand ampersands into "and"
    Replacer& replaceAmpersand();

    // Expand "St" and "St." into "Saint"
    Replacer& replaceSaint();

    // Expand cardinal abbreviations
    Replacer& replaceCardinalAbbrev();

    // Set dest to internal src
    Replacer& set(std::string& dest);
};
}  // namespace cg
