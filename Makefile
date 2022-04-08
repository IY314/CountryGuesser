# Makefile
# Author: Isaac Yee
# Build scripts

CXX := c++
WARN := -Wall -Wextra
STD := -std=c++17
INCLUDE := -Iinclude -Ilib/argparse/include
LIBS := -lncurses

TARGET := bin/cg
OBJECTS := obj/main.o obj/csv.o obj/guess.o obj/game.o obj/util.o

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS) bin
	$(CXX) $(WARN) $(STD) $(LIBS) $(OBJECTS) -o $(TARGET)

obj/%.o: src/%.cc obj
	$(CXX) $(WARN) $(STD) $(INCLUDE) -c $< -o $@

obj:
	@mkdir -p obj

bin:
	@mkdir -p bin

clean:
	@rm -rf $(TARGET) obj
