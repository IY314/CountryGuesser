// main.cc
// Author: Isaac Yee
// Argument parsing and start of program

#include <iostream>

#include <argparse/argparse.hpp>

#include "game.hh"

int main(int argc, char** argv) {
    argparse::ArgumentParser args("cg", "1.0");
    args.add_argument("--tutorial", "-t")
        .help("display a tutorial on launch")
        .default_value(false)
        .implicit_value(true);
    args.add_argument("--nocolor", "-C")
        .help("do not use color")
        .default_value(true)
        .implicit_value(false);
    args.add_argument("--file", "-f")
        .help("specify country CSV file")
        .default_value<std::string>("countries.csv");
    args.add_argument("--requirement", "-r")
        .help("specify how many countries required to be guessed")
        .default_value<size_t>(195)
        .scan<'i', size_t>();
    args.add_argument("--typos", "-y")
        .help("specify how many typos are allowed")
        .default_value<size_t>(10)
        .scan<'i', size_t>();

    try {
        args.parse_args(argc, argv);
    } catch (const std::runtime_error& err) {
        std::cerr << err.what() << std::endl;
        std::cerr << args;
        std::exit(1);
    }

    cg::Game(args).loop();
}
