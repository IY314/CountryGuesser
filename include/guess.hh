#pragma once

#include "csv.hh"

namespace cg {
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

// case insensitive equality functor
class caseInsensitiveEquals {
    std::string a;

   public:
    caseInsensitiveEquals(const std::string& a);

    bool operator()(const std::string& b) const;
};

void replaceAll(std::string& src);

void replaceAmpersand(std::string& src);

void replaceSaint(std::string& src);
}  // namespace cg
