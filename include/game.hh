#pragma once

#include <ncurses.h>

#include "csv.hh"
#include "guess.hh"

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
    unsigned long progBarWidth, progBarFilled;
    long double progress;
    unsigned short progPercent;

    std::string input, msg;
    int ch;
    unsigned long y, x, c;

    enum GuessStatus { gs_note, gs_lose, gs_ok, gs_null } status;

    void playTutorial();

    void getCountries();
    void display();
    void getInput();
    void processGuess();
    void validateGuess(const guess::caseInsensitiveEquals& inputEquals);
    void loseScreen(const std::string& msg) const;

   public:
    Game(bool tutorial);
    ~Game();

    void loop();
};
}  // namespace cg
