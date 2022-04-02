#include <getopt.h>

#include "game.hh"

int main(int argc, char **argv) {
    bool tutorial;
    struct option longOptions[] = {{"tutorial", no_argument, 0, 't'}};
    int optidx, opt;
    while ((opt = getopt_long(argc, argv, "t", longOptions, &optidx)) != -1) {
        switch (opt) {
            case 0:
                if (longOptions[optidx].flag != 0) break;
                switch (longOptions[optidx].val) {
                    case 't':
                        tutorial = true;
                        break;
                }
                break;
            case 't':
                tutorial = true;
                break;
        }
    }
    cg::Game(tutorial).loop();
}
