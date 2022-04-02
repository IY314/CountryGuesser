#include <getopt.h>

#include "game.hh"

#define optswitch        \
    case 't':            \
        tutorial = true; \
        break;           \
    case 'C':            \
        color = false;   \
        break;

int main(int argc, char **argv) {
    bool tutorial = false;
    bool color = true;
    struct option longOptions[] = {{"tutorial", no_argument, 0, 't'},
                                   {"nocolor", no_argument, 0, 'C'}};
    int optidx, opt;
    while ((opt = getopt_long(argc, argv, "tC", longOptions, &optidx)) != -1) {
        switch (opt) {
            case 0:
                if (longOptions[optidx].flag != 0) break;
                switch (longOptions[optidx].val) { optswitch }
                break;

                optswitch
        }
    }
    cg::Game(tutorial, color).loop();
}
