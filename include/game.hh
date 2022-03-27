#pragma once

#include <ncurses.h>

#include "csv.hh"

#define COUNTRIES_PATH "countries.csv"

#define BAR 0
#define PERCENT 1
#define NUMBER 2
#define RECENT 3

namespace cg {
class Game {
    bool hasColor;
    csv::CSVOutput countries;
    std::vector<unsigned long> guessed;

    bool running;

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

    void getCountries();
    void processGuess(std::string& input);
    bool validateGuess(const std::string& guess);
    void loseScreen(const std::string& msg = "You Lose!") const;

   public:
    Game();
    ~Game();

    void loop();
};
}  // namespace cg
