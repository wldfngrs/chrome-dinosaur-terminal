CFLAGS = -std=c++11 -Wall -pedantic

bin/main: bin/main.o bin/game.o bin/player.o bin/objects.o
	g++ -g -o bin/main bin/main.o bin/game.o bin/player.o bin/objects.o $(CFLAGS) -lncurses

bin/main.o: src/main.cpp
	mkdir -p bin
	g++ -g -c -o bin/main.o src/main.cpp $(CFLAGS)

bin/game.o: src/game.cpp src/game.h
	mkdir -p bin
	g++ -g -c -o bin/game.o src/game.cpp $(CFLAGS)

bin/player.o: src/player.cpp src/player.h
	mkdir -p bin
	g++ -g -c -o bin/player.o src/player.cpp $(CFLAGS)

bin/objects.o: src/objects.cpp src/objects.h
	mkdir -p bin
	g++ -g -c -o bin/objects.o src/objects.cpp $(CFLAGS)