#ifndef SIMPLE_GAME_FLX_H
#define SIMPLE_GAME_FLX_H

#include "Objects.hpp"
#include <SFML/Graphics.hpp>

class Game {
public:
    Game(uint width, uint height, std::string title);
    void run( void );

private:
    void readInputs( void );
    void move( void );
    void checkCollision( void );
    void checkVals( void );
    void draw( void );
    void drawCircle( int x, int y, int rad, sf::Color color );
    int calculateY( int x, int rad );
    int calculateX( int y, int rad );


    Coin randomCoin( void );

    int points = 0;
    bool coinsrendered;
    int num_coins = 0;
    sf::Texture player_texture;
    sf::Texture coin_texture;
    sf::Sprite player_sprite;
    Player player;

    sf::Font font;
    sf::Text text;

    //Arrays for drawing
    std::vector<Coin> coins = std::vector<Coin>();
    std::vector<sf::Vertex> render = std::vector<sf::Vertex>();

    sf::RenderWindow window;

};
#endif