all:
	g++ -c src/Game.cpp
	g++ -o game Game.o src/main.cpp -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

run:
	./game
