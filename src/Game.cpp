//
// Created by felix on 23.05.18.
//
#include <vector>
#include <stdio.h>
#include <cmath>
#include "Game.hpp"

Game::Game(uint width, uint height, std::string title) : window({width, height}, title,
                                                                sf::Style::Titlebar | sf::Style::Close) {

    window.setFramerateLimit(60);

    //drawRectangle(50, 50, 50, 50, sf::Color::Red);

    if (player_texture.loadFromFile("assets/rocket.png")) {
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

void Game::run(void) {

    player.angle = 0;

    player.x = window.getSize().x / 2;
    player.y = window.getSize().y / 2;

    coinsrendered = false;

    performanceclock.restart();
    while (window.isOpen()) {
        render.clear();
        readInputs();
        //move();
        checkCollision();
        checkVals();
        draw();
    }
}

void Game::readInputs(void) {
    sf::Event event{};

    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
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

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
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

void Game::checkCollision(void) {

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

void Game::draw(void) {

    if (performanceclock.getElapsedTime().asMilliseconds() >= 1000) {
        performanceclock.restart();
        performance = iterations - olditeration;

        olditeration = iterations;
    }

    window.clear(sf::Color::White);
    render.clear();

    text.setString("Player coordinates"
                   "\nX: " + std::to_string(player.x) +
                   "\nY: " + std::to_string(player.y) +
                   "\nAngle: " + std::to_string(player.angle) +
                   "\nRendered Coins: " + std::to_string(num_coins) +
                   "\nPoints: " + std::to_string(points) +
                   "\nVertices: " + std::to_string(vertices) +
                   "\nIterations; " + std::to_string(iterations) +
                   "\nPerformance: " + std::to_string(performance) + " it/s");

    window.draw(text);

    player_sprite.setPosition(player.x, player.y);
    player_sprite.setRotation(player.angle);

    if (num_coins < 5) {
        coins.push_back(randomCoin());
        num_coins++;
    } else {
        coinsrendered = true;
    }

    for (auto &coin : coins) {
        coin.sprite.setPosition(coin.x, coin.y);
        window.draw(coin.sprite);
    }

    if (iterations % 1000 == 0) {
        renderstatic.clear();
        //Static draw function calls here

        drawCircle(200, 200, 200, sf::Color::Red, true);
        drawCircle(window.getSize().x / 2, window.getSize().y / 2, 200, sf::Color::Black, true);
        //drawCircle(rand() % window.getSize().x, rand() % window.getSize().y, 200, sf::Color::Green);
        drawRectangle(450, 450, 200, 100, sf::Color::Red, true);
    }

    vertices += render.size();

    window.draw(player_sprite);
    window.draw(render.data(), render.size(), sf::Points);
    window.draw(renderstatic.data(), renderstatic.size(), sf::Points);

    window.display();
    iterations++;

}

void Game::drawCircle(float x, float y, int rad, sf::Color color) {
    //Add top most point of circle to render array
    render.push_back(sf::Vertex({x, y - rad}, color));

    //Calculate all points from top-most to right-most point
    for (float i = 1; i <= rad; i += STEPS) {
        render.push_back(sf::Vertex({x + i, y - calculateY(i, rad)}, color));
    }

    //Add right-most point to render array
    render.push_back(sf::Vertex({x + rad, y}, color));

    //Calculate all points from right-most to bottom-most point
    for (float i = 1; i <= rad; i += STEPS) {
        render.push_back(sf::Vertex({x + i, y + calculateY(i, rad)}, color));
    }

    //Add bottom-most point to render array
    render.push_back(sf::Vertex({x, y + rad}, color));

    //Calculate all points from bottom-most to left-most point
    for (float i = 1; i <= rad; i += STEPS) {
        render.push_back(sf::Vertex({x - i, y + calculateY(i, rad)}, color));
    }

    //Add left-most point to render array
    render.push_back(sf::Vertex({x - rad, y}, color));

    for (float i = 1; i <= rad; i += STEPS) {
        render.push_back(sf::Vertex({x - i, y - calculateY(i, rad)}, color));
    }

}

void Game::drawCircle(float x, float y, int rad, sf::Color color, bool stc) {

    if (stc) {
        //Add top most point of circle to render array
        renderstatic.push_back(sf::Vertex({x, y - rad}, color));

        //Calculate all points from top-most to right-most point
        for (float i = 1; i <= rad; i += STEPS) {
            renderstatic.push_back(sf::Vertex({x + i, y - calculateY(i, rad)}, color));
        }

        //Add right-most point to render array
        renderstatic.push_back(sf::Vertex({x + rad, y}, color));

        //Calculate all points from right-most to bottom-most point
        for (float i = 1; i <= rad; i += STEPS) {
            renderstatic.push_back(sf::Vertex({x + i, y + calculateY(i, rad)}, color));
        }

        //Add bottom-most point to render array
        renderstatic.push_back(sf::Vertex({x, y + rad}, color));

        //Calculate all points from bottom-most to left-most point
        for (float i = 1; i <= rad; i += STEPS) {
            renderstatic.push_back(sf::Vertex({x - i, y + calculateY(i, rad)}, color));
        }

        //Add left-most point to render array
        renderstatic.push_back(sf::Vertex({x - rad, y}, color));

        for (float i = 1; i <= rad; i += STEPS) {
            renderstatic.push_back(sf::Vertex({x - i, y - calculateY(i, rad)}, color));
        }
    } else {
        drawCircle(x, y, rad, color);
    }

}

/**
 * Calculate Y for a circle using the Pythagorean theorem
 * rad^2 - x^2 = y^2
 * @param x point x
 * @param rad radius
 * @return point y
 */
int Game::calculateY(float x, int rad) {
    if (rad != 0) {
        return sqrt(pow(rad, 2) - pow(x, 2));
    }
    return -1;
}

/**
 * Draw a rectangle where x and y are the center of the rectangle
 * @param x X-coordinate
 * @param y Y-coordinate
 * @param width width of rectangle
 * @param height height of rectangle
 * @param color color of rectangle
 */
void Game::drawRectangle(float x, float y, float width, float height, sf::Color color) {

    //Draw the top border
    for (float i = 0; i <= width; i += STEPS) {
        render.push_back(sf::Vertex({(x - (0.5 * width)) + i, y - (0.5 * height)}, color));
    }

    //Draw the right border
    for (float i = 0; i <= height; i += STEPS) {
        render.push_back(sf::Vertex({x + (0.5 * width), y - (0.5 * height) + i}, color));
    }

    //Draw the bottom border
    for (float i = 0; i <= width; i += STEPS) {
        render.push_back(sf::Vertex({x - (0.5 * width) + i, y + (0.5 * height)}, color));
    }

    //Draw the left border
    for (float i = 0; i <= height; i += STEPS) {
        render.push_back(sf::Vertex({x - (0.5 * width), y - (0.5 * height) + i}, color));
    }
}

/**
 * Draw a rectangle where x and y are the center of the rectangle
 * @param x X-coordinate
 * @param y Y-coordinate
 * @param width width of rectangle
 * @param height height of rectangle
 * @param color color of rectangle
 */
void Game::drawRectangle(float x, float y, float width, float height, sf::Color color, bool stc) {

    if (stc) {

        //Draw the top border
        for (float i = 0; i <= width; i += STEPS) {
            renderstatic.push_back(sf::Vertex({(x - (0.5 * width)) + i, y - (0.5 * height)}, color));
        }

        //Draw the right border
        for (float i = 0; i <= height; i += STEPS) {
            renderstatic.push_back(sf::Vertex({x + (0.5 * width), y - (0.5 * height) + i}, color));
        }

        //Draw the bottom border
        for (float i = 0; i <= width; i += STEPS) {
            renderstatic.push_back(sf::Vertex({x - (0.5 * width) + i, y + (0.5 * height)}, color));
        }

        //Draw the left border
        for (float i = 0; i <= height; i += STEPS) {
            renderstatic.push_back(sf::Vertex({x - (0.5 * width), y - (0.5 * height) + i}, color));
        }
    } else {
        drawRectangle(x, y, width, height, color);
    }
}

void Game::checkVals(void) {

}

Coin Game::randomCoin(void) {
    Coin coin;

    coin.x = (rand() % window.getSize().x - 40) + 20;
    coin.y = (rand() % window.getSize().y - 40) + 20;

    coin.sprite.setTexture(coin_texture);

    return coin;
}

