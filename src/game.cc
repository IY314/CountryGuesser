// game.cc
// Author: Isaac Yee
// Game I/O and init implementation

#include <cmath>
#include <sstream>

#include <ncurses.h>

#include "game.hh"
#include "util.hh"

cg::Game::Game(bool tutorial, bool color, const std::string& fn, int required) {
    this->required = required;

    // Init ncurses
    initscr();
    raw();
    noecho();
    nonl();
    keypad(stdscr, true);
    refresh();

    // Init color if specified
    hasColor = color && has_colors();
    if (hasColor) {
        start_color();
        use_default_colors();

        init_pair(BAR, COLOR_BLACK, COLOR_WHITE);
        init_pair(PERCENT, COLOR_CYAN, -1);
        init_pair(NUMBER, COLOR_GREEN, -1);
        init_pair(RECENT, COLOR_RED, -1);
    }

    // Set display variables pertaining to dimension
    progBarWidth = COLS - 15;
    mid = LINES / 2;

    // Set filename
    countriesPath = fn;

    if (tutorial) playTutorial();

    getCountries();
}

cg::Game::~Game() {
    // Stop ncurses
    endwin();
}

void cg::Game::popup(const std::string& text, bool cls) const {
    if (cls) clear();

    // Display message
    mvaddstr(mid, 0, text.c_str());

    // Get key and detect for quit key (esc)
    if (getch() == '\e') {
        this->~Game();
        std::exit(0);
    }
}

void cg::Game::win() {
    clear();
    mvaddstr(mid, 0, "You Win!");
    getch();
    running = false;
}

void cg::Game::lose(const std::string& text) {
    clear();
    mvaddstr(mid, 0, text.c_str());
    mvaddstr(mid + 1, 0, "Final Score: ");
    addstr(std::to_string(guessed.size()).c_str());
    getch();
    running = false;
}

void cg::Game::showDisplay(unsigned long start, unsigned long end,
                           const std::string& text) {
    clear();
    display();
    mvaddch(0, start, '|');
    mvaddch(1, start, '+');
    for (unsigned long i = start + 1; i < end; ++i) {
        mvaddch(1, i, '-');
    }
    mvaddch(0, end, '|');
    mvaddch(1, end, '+');
    popup(text, false);
}

void cg::Game::playTutorial() {
    popup(
        "Welcome to CountryGuesser! Press any key to continue, or escape to "
        "quit.");
    popup(
        "The objective of this game is to guess all 195 countries of the "
        "world.");
    popup("If you do not enter a valid country, you lose.");
    popup(
        "Don't worry though. I have been built to be lenient with your "
        "answer.");
    popup(
        "For example, if you type in 'america' instead of 'America,' it still "
        "counts...");
    popup(
        "or if you type in 'St. Patrick's Day' instead of 'Saint Patrick's "
        "Day.'");
    popup("You only need to use ASCII letters; there will not be diacritics.");
    popup("However, I suppose I should show you around...");

    // Point to different parts of the display
    showDisplay(0, progBarWidth - 1, "This is the progress bar.");
    showDisplay(progBarWidth, COLS - 10, "This is the percentage.");
    showDisplay(COLS - 9, COLS - 1,
                "And this is the number of countries you have guessed.");

    popup("Good luck!");
}

void cg::Game::getCountries() {
    std::ifstream is(countriesPath);
    countries = csv::readCSV(is);
    is.close();
}

void cg::Game::display() {
    // Progress bar
    progress = guessed.size() / 195.0;
    progBarFilled = ceil(progress * progBarWidth);
    progPercent = ceil(progress * 100);

    util__addcolor(
        BAR,
        {
            for (unsigned long i = 1; i <= progBarFilled; ++i)
                mvaddch(0, i, '=');
        },
        hasColor);

    // Percentage
    util__addcolor(
        PERCENT,
        {
            mvaddstr(0, COLS - 14, std::to_string(progPercent).c_str());
            addch('%');
        },
        hasColor);

    // Number
    util__addcolor(
        NUMBER,
        {
            mvaddstr(0, COLS - 8, std::to_string(guessed.size()).c_str());
            addch('/');
            addstr(std::to_string(required).c_str());
        },
        hasColor);

    // Recent guesses
    move(LINES / 4, 0);
    refresh();
    util__addcolor(
        RECENT,
        {
            if (guessed.size() >= 5)  // Display 5 most recent guesses
                for (auto i = guessed.end() - 5; i != guessed.end(); ++i)
                    mvaddstr(LINES / 4 + guessed.end() - i, 0,
                             countries.at(*i).at(0).c_str());

            else  // Display ALL guesses
                for (auto i = guessed.begin(); i != guessed.end(); ++i)
                    mvaddstr(LINES / 4 + guessed.end() - i, 0,
                             countries.at(*i).at(0).c_str());
        },
        hasColor);
}

void cg::Game::getInput() {
    // Clear previous guess from variable
    guess.clear();

    // Set cursor position
    cursor = 0;

    // Prompt the player
    mvaddstr(mid, 0, "Enter a country: ");

    // Detect for typing
    bool typing = true;
    while (typing) {
        // Clear from after the prompt to end of line
        move(mid, 17);
        clrtoeol();

        // Display current input (this and the clearing is required for good
        // backspace implementation)
        addstr(guess.c_str());

        // Move terminal cursor to stored position
        move(mid, cursor + 17);

        switch (ch = getch()) {
            // Delete last character from input
            case KEY_BACKSPACE:
            case KEY_DC:
            case 127:
            case '\b':
                if (cursor > 0) {
                    guess.erase(cursor - 1, 1);
                    --cursor;
                }
                break;

            // Submit input
            case '\n':
            case '\r':
                typing = false;
                break;

            // Quit game
            case '\e':
                typing = false;
                lose("");
                break;

            // Move
            case KEY_LEFT:
                if (cursor > 0) --cursor;
                break;
            case KEY_RIGHT:
                if (cursor < guess.size()) ++cursor;
                break;

            // Add to input
            default:
                guess.insert(cursor, std::string(1, ch));
                ++cursor;
                break;
        }
    }

    // Move cursor to space immediately after "Enter a country: "
    move(mid, 17);
}

void cg::Game::loop() {
    running = true;
    while (running) {
        clear();
        display();
        getInput();

        if (running) {
            if (util::reduce(guess) == "") continue;
            // Expand all abbreviations, etc
            Replacer(guess).replaceAll().set(guess);
            processGuess();
        }
    }
}
