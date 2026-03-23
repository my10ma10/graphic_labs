.PHONY: all clean

all: main

main: main.o drawers.o
	g++ main.o drawers.o -o main -lGLEW -lGL -lGLU -ldl -lglfw -g

main.o: main.cpp drawers.hpp
	g++ -c main.cpp -o main.o

drawers.o: drawers.cpp drawers.hpp
	g++ -c drawers.cpp -o drawers.o
	

clean:
	rm -f main *.o