main: main.o Puzzle_node.o
	g++ -std=c++11 -o main main.o Puzzle_node.o

main.o: main.cpp
	g++ -std=c++11 -c main.cpp

Puzzle_node.o: Puzzle_node.cpp Puzzle_node.h
	g++ -std=c++11 -c Puzzle_node.cpp

clean:
	rm main main.o Puzzle_node.o
