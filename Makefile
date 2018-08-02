CC=g++
CFLAGS=-g -Wall

all: tb.cpp
	$(CC) $(CFLAGS) tb.cpp -o game tictactoe.h tictactoe.cpp

clean:
	rm -f game *.out
