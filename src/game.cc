#include <algorithm>
#include <cmath>
#include <sstream>

#include "game.hh"
#include "util.hh"

cg::Game::Game() {
    // Init ncurses
    initscr();
    raw();
    noecho();
    nonl();
    keypad(stdscr, true);
    refresh();

    if (has_colors()) {
        start_color();
        use_default_colors();

        init_pair(BAR, COLOR_BLACK, COLOR_WHITE);
        init_pair(PERCENT, COLOR_CYAN, -1);
        init_pair(NUMBER, COLOR_GREEN, -1);
        init_pair(RECENT, COLOR_RED, -1);

        hasColor = true;
    } else {
        hasColor = false;
    }

    getCountries();
}

cg::Game::~Game() { endwin(); }

void cg::Game::getCountries() {
    std::ifstream is(COUNTRIES_PATH);
    countries = csv::readCSV(is);
}

void cg::Game::loop() {
    unsigned long progressBarWidth = COLS - 15;

    running = true;
    while (running) {
        clear();

        // Progress bar
        long double progress = guessed.size() / 195.0;
        unsigned long progressBarFilled = ceil(progress * progressBarWidth);
        unsigned short progressPercent = ceil(progress * 100);
        if (hasColor) attron(COLOR_PAIR(BAR));
        for (unsigned long i = 1; i <= progressBarFilled; ++i) {
            mvaddch(0, i, '=');
        }
        if (hasColor) {
            attroff(COLOR_PAIR(BAR));
            attron(COLOR_PAIR(PERCENT));
        }

        // Percentage
        mvaddstr(0, COLS - 14, std::to_string(progressPercent).c_str());
        addch('%');
        if (hasColor) {
            attroff(COLOR_PAIR(PERCENT));
            attron(COLOR_PAIR(NUMBER));
        }

        // Number
        mvaddstr(0, COLS - 8, std::to_string(guessed.size()).c_str());
        addstr("/195");
        if (hasColor) attroff(COLOR_PAIR(NUMBER));

        // Recent guesses
        if (hasColor) attron(COLOR_PAIR(RECENT));
        move(LINES / 4, 0);
        refresh();
        if (guessed.size() >= 5) {
            for (auto i = guessed.end() - 5; i != guessed.end(); ++i) {
                mvaddstr(LINES / 4 + guessed.end() - i, 0,
                         countries.at(*i).at(0).c_str());
            }
        } else {
            for (auto i = guessed.begin(); i != guessed.end(); ++i) {
                mvaddstr(LINES / 4 + guessed.end() - i, 0,
                         countries.at(*i).at(0).c_str());
            }
        }
        if (hasColor) attroff(COLOR_PAIR(RECENT));

        // Get input
        mvaddstr(LINES / 2, 0, "Enter a country: ");
        bool typing = true;
        std::string input;
        int ch, y, x;
        while (typing) {
            // Clear displayed input and replace it with stored string
            getyx(stdscr, y, x);
            clrtoeol();
            move(y, x);
            addstr(input.c_str());

            switch (ch = getch()) {
                // Delete last character from input
                case KEY_BACKSPACE:
                case KEY_DC:
                case 127:
                case '\b':
                    if (input.size() > 0) input.pop_back();
                    break;

                // Submit input
                case '\n':
                case '\r':
                    typing = false;
                    break;

                // Quit game
                case '\e':
                    typing = false;
                    running = false;
                    loseScreen();
                    break;

                // Add to input
                default:
                    input.push_back(ch);
                    break;
            }

            // Move cursor to space immediately after "Enter a country: "
            move(y, 17);
        }

        processGuess(input);
    }
}

void cg::Game::loseScreen(const std::string& msg) const {
    clear();
    mvaddstr(LINES / 2, 0, msg.c_str());
    mvaddstr(LINES / 2 + 1, 0, "Final Score: ");
    addstr(std::to_string(guessed.size()).c_str());
    getch();
}

void cg::Game::processGuess(std::string& input) {
    input = util::reduce(input);

    // Various easter eggs :)
    if (running &&
        std::any_of(
            easterEggs.begin(), easterEggs.end(), [input](std::string e) {
                return std::equal(input.begin(), input.end(), e.begin(),
                                  e.end(), [](char a, char b) {
                                      return std::tolower(a) == std::tolower(b);
                                  });
            })) {
        clear();
        mvaddstr(LINES / 2, 0, "Nice one there, wisebutt.");
        getch();
    } else if (running &&
               std::any_of(metaEasterEggs.begin(), metaEasterEggs.end(),
                           [input](std::string e) {
                               return std::equal(input.begin(), input.end(),
                                                 e.begin(), e.end(),
                                                 [](char a, char b) {
                                                     return std::tolower(a) ==
                                                            std::tolower(b);
                                                 });
                           })) {
        for (auto it = metaEasterEggs.begin(); it != metaEasterEggs.end();
             ++it) {
            if (std::equal((*it).begin(), (*it).end(), input.begin(),
                           input.end(), [](char a, char b) {
                               return std::tolower(a) == std::tolower(b);
                           })) {
                if (it == metaEasterEggs.end() - 1) {
                    loseScreen("I warned you, didn't I? YOU LOSE!");
                    running = false;
                } else {
                    clear();
                    mvaddstr(LINES / 2, 0,
                             metaEasterEggs.at(it - metaEasterEggs.begin() + 1)
                                 .c_str());
                    getch();
                }
            }
        }
    } else if (running &&
               std::equal(youLose.begin(), youLose.end(), input.begin(),
                          input.end(), [](char a, char b) {
                              return std::tolower(a) == std::tolower(b);
                          })) {
        loseScreen("No, YOU lose!");
        running = false;
    }

    // Check validity of guess
    else if (running && !validateGuess(input)) {
        loseScreen();
        running = false;
    }
}

bool cg::Game::validateGuess(const std::string& guess) {
    bool foundCountry = false;

    // Iterate through all countries
    for (auto i = countries.begin(); i != countries.end(); ++i) {
        // Some countries have multiple names, so each country must also be
        // iterated through
        for (auto ci = (*i).begin(); ci != (*i).end(); ++ci) {
            // Case-insensitive check of guess and country name
            if (std::equal((*ci).begin(), (*ci).end(), guess.begin(),
                           guess.end(), [](char a, char b) {
                               return std::tolower(a) == std::tolower(b);
                           })) {
                unsigned long idx = i - countries.begin();
                if (std::find(guessed.begin(), guessed.end(), idx) !=
                    guessed.end()) {
                    clear();
                    mvaddstr(LINES / 2, 0,
                             "That country has already been guessed!");
                    getch();
                } else {
                    guessed.push_back(idx);
                }
                foundCountry = true;
                break;
            }
        }

        if (foundCountry) break;
    }

    return foundCountry;
}
