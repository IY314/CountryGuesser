WFLAGS := -Wall -Werror -Wextra -std=c++17
IFLAGS := -Iinclude
LFLAGS := -lncurses
OFLAGS := -c ${WFLAGS} ${IFLAGS}
CXXFLAGS := -o bin/cg ${LFLAGS}

.PHONY: all link clean-o clean

all: link

link: bin/main.o bin/csv.o bin/game.o bin/util.o bin/guess.o
	${CXX} $^ ${CXXFLAGS}

bin/%.o: src/%.cc bin
	${CXX} $< -o $@ ${OFLAGS}

bin:
	mkdir -p bin

clean-o:
	rm bin/*.o

clean: clean-o
	rm bin/cg
