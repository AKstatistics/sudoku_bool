SHELL=/bin/sh
.SUFFIXES:
.SUFFIXES: .cpp .o
CC=g++
CFLAGS=-c -Wall -std=c++11
LDFLAGS=-lz
SOURCES=main.cpp Sudoku.cpp Undo.cpp
HEADERS=Sudoku.h Undo.h
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=play_sudoku

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) -Wall -std=c++11 $(OBJECTS) $(LDFLAGS) -o $@ 

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp

Sudoku.o: Sudoku.cpp Sudoku.h
	$(CC) $(CFLAGS) Sudoku.cpp

Undo.o: Undo.cpp Undo.h
	$(CC) $(CFLAGS) Undo.cpp


#.cpp.o:
#	$(CC) $(CFLAGS) $< -o $@

clean:
	rm *o play_sudoku
