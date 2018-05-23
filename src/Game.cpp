//
// Created by felix on 23.05.18.
//
#include <vector>
#include <stdio.h>
#include <cmath>
#include "Game.hpp"

Game::Game( uint width, uint height, std::string title ) : window({width, height}, title, sf::Style::Titlebar | sf::Style::Close) {

    window.setFramerateLimit(30);

    if(player_texture.loadFromFile("assets/rocket.png")) {
        player_texture.setSmooth(true);
        player_texture.setRepeated(false);
        player_sprite.setTexture(player_texture);
    }

    if (coin_texture.loadFromFile("assets/asteroid.png")) {
        coin_texture.setSmooth(true);
        coin_texture.setRepeated(false);
    }

    font.loadFromFile("assets/arial.ttf");
    text.setFont(font);
    text.setFillColor(sf::Color::White);
    text.setOutlineColor(sf::Color::Black);
    text.setOutlineThickness(3);
    text.setCharacterSize(15);
    text.setPosition(5, 5);
}

void Game::run( void ) {

    player.angle = 0;

    player.x = window.getSize().x / 2;
    player.y = window.getSize().y / 2;

    coinsrendered = false;

    while(window.isOpen()) {
        render.clear();
        readInputs();
        //move();
        checkCollision();
        checkVals();
        draw();
    }
}

void Game::readInputs( void ) {
    sf::Event event{};

    while(window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
	    if (player.angle < 270) {
		for (int i = player.angle; i <= 270; i++) {
			player.angle = i;
			sf::milliseconds(500);
		}
	    } else {
		for (int i = player.angle; i >= 270; i--) {
			player.angle = i;
			sf::milliseconds(500);
		}	
	    }
	    //player.angle = 270.0f;
        if (player.y >= 40) {
            player.y -= 20;
        }

    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
	    player.angle = 90.0f;
        if (player.y <= window.getSize().y - 40) {
            player.y += 20;
        }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
	    player.angle = 180.0f;
        if (player.x >= 40) {
            player.x -= 20;
        }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
	    player.angle = .0f;
        if (player.x <= window.getSize().x - 40) {
            player.x += 20;
        }
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
        if (player.angle <= 320) {
            player.angle += 90;
        } else {
            player.angle = 0;
        }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q) || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
        window.close();
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::R) && coinsrendered) {
        player.x = window.getSize().x / 2;
        player.y = window.getSize().y / 2;
        player.angle = .0f;

        coins.clear();
        num_coins = 0;
        points = 0;

        coinsrendered = false;

    }

}

void Game::checkCollision( void ) {

    int i = 0;

    for (auto &coin : coins) {
        if (coin.sprite.getGlobalBounds().intersects(player_sprite.getGlobalBounds())) {
            coins.erase(coins.begin() + i);
            num_coins--;
            points++;
            coinsrendered = false;
        }
        i++;
    }


}

void Game::draw( void ) {

    window.clear(sf::Color::White);

    text.setString("Player coordinates"
                   "\nX: " + std::to_string(player.x) +
                   "\nY: " + std::to_string(player.y) +
                   "\nAngle: " + std::to_string(player.angle) +
                   "\nRendered Coins: " + std::to_string(num_coins) +
                   "\nPoints: " + std::to_string(points));

    window.draw(text);

    player_sprite.setPosition(player.x, player.y);
    player_sprite.setRotation(player.angle);

    if (num_coins < 5) {
        coins.push_back(randomCoin());
        num_coins++;
    } else {
        coinsrendered = true;
    }

    for(auto &coin : coins) {
        coin.sprite.setPosition(coin.x, coin.y);
        window.draw(coin.sprite);
    }

    window.draw(player_sprite);

    window.display();

}

void Game::drawCircle( int x, int y, int rad, sf::Color color ) {
    //Add top most point of circle to render array
    render.push_back(sf::Vertex({x, y + rad}, color));

    //Calculate all points from top-most to right-most point
    for(int i = 1; i <= rad; i++) {
        render.push_back(sf::Vertex({i, calculateY(x, rad)}, color));
    }

    //Add right-most point to render array
    render.push_back(sf::Vertex({x + rad, y}, color));

    //Calculate all points from right-most to bottom-most point


}

/**
 * Calculate Y for a circle using the Pythagorean theorem
 * @param x point x
 * @param rad radius
 * @return point y
 */
int Game::calculateY( int x, int rad ) {

}

/**
 * Calculate X for a circle using the Pythagorean theorem
 * @param y point y
 * @param rad radius
 * @return point x
 */
int Game::calculateX( int y, int rad ) {

}

void Game::checkVals( void ) {

}

Coin Game::randomCoin( void ) {
    Coin coin;

    coin.x = (rand() % window.getSize().x - 40) + 20;
    coin.y = (rand() % window.getSize().y - 40) + 20;

    coin.sprite.setTexture(coin_texture);

    return coin;
}

