#include <algorithm>
#include <cmath>
#include <sstream>

#include "game.hh"

cg::Game::Game() {
    initscr();
    raw();
    noecho();
    nonl();
    keypad(stdscr, true);
    refresh();

    if (has_colors()) {
        start_color();

        init_pair(10, COLOR_CYAN, COLOR_BLACK);
        init_pair(20, COLOR_GREEN, COLOR_BLACK);
        init_pair(30, COLOR_BLACK, COLOR_WHITE);

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

    bool running = true;
    while (running) {
        clear();
        // Progress bar
        long double progress = guessed.size() / 195.0;
        unsigned long progressBarFilled = ceil(progress * progressBarWidth);
        unsigned short progressPercent = ceil(progress * 100);
        if (hasColor) attron(COLOR_PAIR(30));
        for (unsigned long i = 1; i <= progressBarFilled; ++i) {
            mvaddch(0, i, '=');
        }
        if (hasColor) {
            attroff(COLOR_PAIR(30));
            attron(COLOR_PAIR(10));
        }
        mvaddstr(0, COLS - 14, std::to_string(progressPercent).c_str());
        mvaddch(0, COLS - 11, '%');
        if (hasColor) {
            attroff(COLOR_PAIR(10));
            attron(COLOR_PAIR(20));
        }
        mvaddstr(0, COLS - 8, std::to_string(guessed.size()).c_str());
        mvaddstr(0, COLS - 5, "/195");
        if (hasColor) attroff(COLOR_PAIR(20));

        // Get input
        mvaddstr(LINES / 2, 0, "Enter a country: ");
        bool typing = true;
        std::string input;
        int ch, y, x;
        while (typing) {
            getyx(stdscr, y, x);
            clrtoeol();
            move(y, x);
            addstr(input.c_str());
            switch (ch = getch()) {
                case KEY_BACKSPACE:
                case KEY_DC:
                case 127:
                case '\b':
                    input.pop_back();
                    break;
                case '\n':
                case '\r':
                    typing = false;
                    break;
                case '\e':
                    typing = false;
                    running = false;
                    loseScreen();
                    break;
                default:
                    input.push_back(ch);
                    break;
            }
            move(y, 17);
        }
        if (running && !processGuess(input)) {
            loseScreen();
            running = false;
        }
    }
}

void cg::Game::loseScreen() {
    clear();
    mvaddstr(LINES / 2, 0, "You Lose!");
    mvaddstr(LINES / 2 + 1, 0, "Final Score: ");
    addstr(std::to_string(guessed.size()).c_str());
    getch();
}

bool cg::Game::processGuess(const std::string& guess) {
    bool foundCountry = false;

    for (auto i = countries.begin(); i != countries.end(); ++i) {
        for (auto ci = (*i).begin(); ci != (*i).end(); ++ci) {
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
