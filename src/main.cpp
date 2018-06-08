#include <iostream>
#include "Game.hpp"

int main( int argc, char **argv) {
    Game game(900, 900, "SimpleGame");

    game.run();
    return 0;
}