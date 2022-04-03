#include <getopt.h>
#include <iostream>

#include "game.hh"

// Version numbers
#define VMAJOR 1
#define VMINOR 0

// Options are processed twice (long and short), so this macro "reuses" the code
#define optswitch                                               \
    case 't':                                                   \
        tutorial = true;                                        \
        break;                                                  \
    case 'C':                                                   \
        color = false;                                          \
        break;                                                  \
    case 'v':                                                   \
        std::cout << "cg v" << VMAJOR << '.' << VMINOR << '\n'; \
        std::exit(0);                                           \
        break;

int main(int argc, char **argv) {
    bool tutorial = false;
    bool color = true;
    struct option longOptions[] = {{"tutorial", no_argument, 0, 't'},
                                   {"nocolor", no_argument, 0, 'C'},
                                   {"version", no_argument, 0, 'v'}};
    int optidx, opt;

    // Parse options
    while ((opt = getopt_long(argc, argv, "tCv", longOptions, &optidx)) != -1) {
        switch (opt) {
            case 0:
                if (longOptions[optidx].flag != 0) break;
                switch (longOptions[optidx].val) { optswitch }
                break;

                optswitch
        }
    }

    // Start game
    cg::Game(tutorial, color).loop();
}
