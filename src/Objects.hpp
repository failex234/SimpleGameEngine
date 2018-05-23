
#include <SFML/Graphics/Sprite.hpp>

typedef struct player {
    int x;
    int y;
    float angle;
} Player;

typedef struct coin {
    int x;
    int y;
    bool collected;
    sf::Sprite sprite;
} Coin;