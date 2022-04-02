#pragma once

#include "csv.hh"

namespace cg::guess {
const std::vector<std::string> easterEggs{
    "That country has already been guessed!",
    "A country",
    "This country",
    "That country",
    "Enter a country:",
    "Enter a country"};

const std::vector<std::string> metaEasterEggs{
    "Nice one there, wisebutt.",
    "You think you're funny?",
    "Ok, this is getting out of hand.",
    "Are you actually trying to play the game?",
    "Ok, I'm warning you. DO NOT TYPE THIS IN.",
    "Why'd you type it in?",
    "I'm done with you. If you type this in, you die."};

const std::string youLose = "You Lose!";

// case insensitive equality functor
class caseInsensitiveEquals {
    std::string a;

   public:
    caseInsensitiveEquals(const std::string& a);

    bool operator()(const std::string& b) const;
};

void replaceSaintAbbrev(std::string& src);
}  // namespace cg::guess
