#include <algorithm>
#include <cmath>
#include <sstream>

#include "game.hh"
#include "guess.hh"
#include "util.hh"

cg::Game::Game(bool tutorial) {
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

    progBarWidth = COLS - 15;

    if (tutorial) playTutorial();

    getCountries();
}

cg::Game::~Game() { endwin(); }

void cg::Game::popup(const std::string& text, bool cls) const {
    if (cls) clear();
    mvaddstr(LINES / 2, 0, text.c_str());
    if (getch() == '\e') {
        this->~Game();
        std::exit(0);
    }
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
    showDisplay(0, progBarWidth - 1, "This is the progress bar.");
    showDisplay(progBarWidth, COLS - 10, "This is the percentage.");
    showDisplay(COLS - 9, COLS - 1,
                "And this is the number of countries you have guessed.");
    popup("Good luck!");
}

void cg::Game::getCountries() {
    std::ifstream is(COUNTRIES_PATH);
    countries = csv::readCSV(is);
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
            addstr("/195");
        },
        hasColor);

    // Recent guesses
    move(LINES / 4, 0);
    refresh();
    util__addcolor(
        RECENT,
        {
            if (guessed.size() >= 5)
                for (auto i = guessed.end() - 5; i != guessed.end(); ++i)
                    mvaddstr(LINES / 4 + guessed.end() - i, 0,
                             countries.at(*i).at(0).c_str());
            else
                for (auto i = guessed.begin(); i != guessed.end(); ++i)
                    mvaddstr(LINES / 4 + guessed.end() - i, 0,
                             countries.at(*i).at(0).c_str());
        },
        hasColor);
}

void cg::Game::getInput() {
    input.clear();
    c = 0;
    mvaddstr(LINES / 2, 0, "Enter a country: ");
    bool typing = true;
    while (typing) {
        // Clear displayed input and replace it with stored string
        y = getcury(stdscr);
        move(y, 17);
        clrtoeol();
        addstr(input.c_str());
        move(y, c + 17);

        switch (ch = getch()) {
            // Delete last character from input
            case KEY_BACKSPACE:
            case KEY_DC:
            case 127:
            case '\b':
                if (c > 0) {
                    input.erase(c - 1, 1);
                    --c;
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
                running = false;
                loseScreen("");
                break;

            // Move
            case KEY_LEFT:
                if (c > 0) --c;
                break;
            case KEY_RIGHT:
                if (c < input.size()) ++c;
                break;

            // Add to input
            default:
                input.insert(c, std::string(1, ch));
                ++c;
                break;
        }
    }

    // Move cursor to space immediately after "Enter a country: "
    move(y, 17);
}

void cg::Game::loop() {
    running = true;
    while (running) {
        clear();
        display();
        getInput();

        if (running) {
            if (util::reduce(input) == "") continue;
            cg::guess::replaceAll(input);
            processGuess();
            switch (status) {
                case gs_note:
                    clear();
                    mvaddstr(LINES / 2, 0, msg.c_str());
                    getch();
                    break;
                case gs_lose:
                    loseScreen(msg);
                    running = false;
                    break;
                case gs_ok:
                    guessed.push_back(std::stoul(msg));
                    break;
            }
        }
    }
}

void cg::Game::loseScreen(const std::string& msg) const {
    clear();
    mvaddstr(LINES / 2, 0, msg.c_str());
    mvaddstr(LINES / 2 + 1, 0, "Final Score: ");
    addstr(std::to_string(guessed.size()).c_str());
    getch();
}
