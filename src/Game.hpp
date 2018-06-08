#ifndef SIMPLE_GAME_FLX_H
#define SIMPLE_GAME_FLX_H

#include "Objects.hpp"
#include <SFML/Graphics.hpp>

#define STEPS .009

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
    void drawCircle( float x, float y, int rad, sf::Color color );
    void drawCircle( float x, float y, int rad, sf::Color color, bool stc );
    void drawRectangle( float x, float y, float width, float height, sf::Color color );
    void drawRectangle( float x, float y, float width, float height, sf::Color color, bool stc );
    int calculateY( float x, int rad );


    Coin randomCoin( void );

    int points = 0;
    uint64_t vertices = 0;
    uint64_t iterations = 0;
    int olditeration = 0;
    bool coinsrendered;
    int num_coins = 0;
    double performance = 0;
    int performancecount = 0;
    sf::Clock performanceclock;
    sf::Texture player_texture;
    sf::Texture coin_texture;
    sf::Sprite player_sprite;
    Player player;

    sf::Font font;
    sf::Text text;

    //Arrays for drawing
    std::vector<Coin> coins = std::vector<Coin>();
    std::vector<sf::Vertex> render = std::vector<sf::Vertex>();

    //Static array so that vertices don't have to be calculated over and over
    std::vector<sf::Vertex> renderstatic = std::vector<sf::Vertex>();

    sf::RenderWindow window;

};
#endif