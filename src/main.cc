#include <iostream>

#include <argparse/argparse.hpp>

#include "game.hh"

int main(int argc, char **argv) {
    argparse::ArgumentParser args("cg", "1.0");
    args.add_argument("--tutorial", "-t")
        .help("display a tutorial on launch")
        .default_value(false)
        .implicit_value(true);
    args.add_argument("--nocolor", "-C")
        .help("do not use color")
        .default_value(false)
        .implicit_value(true);
    args.add_argument("--file", "-f")
        .help("specify country CSV file")
        .default_value("countries.csv");
}
