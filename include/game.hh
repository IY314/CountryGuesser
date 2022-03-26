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

    void getCountries();
    bool processGuess(const std::string& guess);
    void loseScreen();

   public:
    Game();
    ~Game();

    void loop();
};
}  // namespace cg
