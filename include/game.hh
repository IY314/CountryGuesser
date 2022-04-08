// game.hh
// Author: Isaac Yee
// Game declarations

#pragma once

#include "guess.hh"

// ncurses color constants
#define BAR 0
#define PERCENT 1
#define NUMBER 2
#define RECENT 3

namespace cg {
class Game {
    // Init variables
    std::string countriesPath;
    bool hasColor, running;
    size_t required;

    // Game data
    std::vector<std::vector<std::string>> countries;
    std::vector<unsigned long> guessed;

    // Progress bar variables
    unsigned long progBarWidth, progBarFilled, mid;
    long double progress;
    unsigned short progPercent;

    // Per-guess variables
    std::string guess;
    unsigned long cursor;
    int ch;

    // Show a "popup" to inform the player
    void popup(const std::string& text, bool cls = true) const;

    // Show a win screen
    void win();

    // Show a lose screen
    void lose(const std::string& text);

    // Generate an "arrow" to point to different parts of the display
    void showDisplay(unsigned long start, unsigned long end,
                     const std::string& text);

    // Play the tutorial
    void playTutorial();

    // Get countries from CSV file
    void getCountries();

    // Display progress and recent guesses
    void display();

    // Get input from player
    void getInput();

    // Process player input
    void processGuess();

    // Check input against country data
    void validateGuess();

   public:
    // Initialize the game
    Game(bool tutorial, bool color, const std::string& fn, size_t required);

    // Deinitialize the game (stop ncurses)
    ~Game();

    // Start the game
    void loop();
};
}  // namespace cg
