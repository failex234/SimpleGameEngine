all:
	g++ -c -g src/Game.cpp
	g++ -o game Game.o -g src/main.cpp -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

run:
	./game
