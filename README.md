# Country Guessing Game
Uh yea. A country guessing game because I'm a huge nerd and memorized most of the 195 countries of the world.

## Premise
The premise of the game is to guess a set amount of countries in the world. There are some alternate names for countries, but I will try to keep the list updated. There is a progress bar, a percentage, and the number of countries guessed on top.

If you enter an unknown country, the game is over (but I am planning to add something to make it more fun).

## Installation
```sh
git clone https://github.com/IY314/CountryGuesser.git --recurse-submodules
cd CountryGuesser
make
```

## Flags
- `--tutorial`, `-t`: display a tutorial on launch (default `false`)
- `--nocolor`, `-C`: do not use color (ignored on terminals that do not support color, default `false`)
- `--file`, `-f` `[filename]`: specify country CSV file to be used (default `countries.csv`)
- `--requirement`, `-r` `[requirement]`: specify how many countries required to be guessed to win (default 195)
