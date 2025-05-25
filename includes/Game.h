#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include "Paddle.h"
#include "Ball.h"
#include "Block.h"
#include "Bonus.h"

class Game {
public:
    Game();
    void run();

private:
    void initBlocks();
    void checkCollisions();
    void spawnBonus(const sf::Vector2f& pos);
    void applyBonus(BonusType type);

    sf::RenderWindow window;
    Paddle paddle;
    Ball ball;
    std::vector<Block> blocks;
    std::vector<Bonus> bonuses;

    int lives = 3;
    int score = 0;
    sf::Font font;
    sf::Text scoreText;
    sf::Text livesText;
};
