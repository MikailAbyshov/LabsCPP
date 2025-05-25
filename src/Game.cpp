#include "../includes/Game.h"
#include <random>
#include <iostream>
#include <filesystem>
#include <set>

enum class CollisionDirection {
    NONE,
    LEFT,
    RIGHT,
    TOP,
    BOTTOM
};

CollisionDirection getCollisionDirection(const sf::FloatRect& ballBounds, const sf::FloatRect& blockBounds) {
    sf::Vector2f centerBall = {ballBounds.left + ballBounds.width / 2.f, ballBounds.top + ballBounds.height / 2.f};
    sf::Vector2f centerBlock = {blockBounds.left + blockBounds.width / 2.f, blockBounds.top + blockBounds.height / 2.f};

    float dx = (centerBall.x - centerBlock.x);
    float dy = (centerBall.y - centerBlock.y);

    float width = (ballBounds.width + blockBounds.width) / 2.f;
    float height = (ballBounds.height + blockBounds.height) / 2.f;

    float crossWidth = width * dy;
    float crossHeight = height * dx;

    if (std::abs(dx) < width && std::abs(dy) < height) {
        if (dy > 0 && crossWidth > crossHeight) return CollisionDirection::TOP;
        if (dy < 0 && crossWidth > -crossHeight) return CollisionDirection::BOTTOM;
        if (dx > 0 && crossHeight > crossWidth) return CollisionDirection::LEFT;
        if (dx < 0 && crossHeight < -crossWidth) return CollisionDirection::RIGHT;
    }

    return CollisionDirection::NONE;
}

Game::Game()
        : window(sf::VideoMode(800, 600), "Arkanoid"), paddle(window.getSize().x / 2.f, 550.f),
          ball(400.f, 300.f), font(), scoreText("Score: 0", font, 24), livesText("Lives: 3", font, 24) {

    std::cout << "Current working directory: " << std::filesystem::current_path() << std::endl;

    if (!font.loadFromFile("../assets/fonts/DejaVuSans.ttf")) {
        std::cerr << "ERROR: Could not load font file 'assets/fonts/DejaVuSans.ttf'" << std::endl;
    }

    scoreText.setPosition(10, 10);
    livesText.setPosition(700, 10);
    scoreText.setFillColor(sf::Color::White);
    livesText.setFillColor(sf::Color::White);

    initBlocks();
}

void Game::run() {
    window.setFramerateLimit(60);
    sf::Clock clock;

    float bottomLineY = 595.f;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        paddle.update();
        ball.update();

        if (ball.getPosition().y > window.getSize().y) {
            if (hasBottomBonus) {
                ball.reverseVelocityY();
                ball.setPosition(ball.getPosition().x, bottomLineY - ball.getRadius() * 2);
                hasBottomBonus = false;
            } else {
                lives--;
                livesText.setString("Lives: " + std::to_string(lives));
                ball.reset();
                if (lives <= 0) window.close();
            }
        }

        checkCollisions();

        window.clear();

        window.draw(paddle);
        window.draw(ball);

        for (auto& block : blocks) {
            window.draw(block);
        }

        for (auto& bonus : bonuses) {
            window.draw(bonus);
        }

        window.draw(scoreText);
        window.draw(livesText);

        window.display();
    }
}

void Game::initBlocks() {
    std::mt19937 rng(std::random_device{}());
    const int rows = 5;
    const int cols = 10;
    const float blockSizeX = 70.f;
    const float blockSizeY = 30.f;

    std::set<std::pair<int, int>> usedPositions;

    std::vector<BlockType> blockTypes = {
            BlockType::NORMAL,
            BlockType::UNBREAKABLE,
            BlockType::BONUS,
            BlockType::FAST_BALL,
            BlockType::MULTIHIT
    };

    for (const auto& type : blockTypes) {
        int row, col;
        do {
            row = std::uniform_int_distribution<>(0, rows - 1)(rng);
            col = std::uniform_int_distribution<>(0, cols - 1)(rng);
        } while (usedPositions.count({row, col}));
        usedPositions.insert({row, col});

        float x = col * blockSizeX + 50;
        float y = row * blockSizeY + 50;
        blocks.emplace_back(x, y, type);
    }

    std::uniform_int_distribution<> distUnbreakable(1, 2);
    int extraUnbreakable = distUnbreakable(rng);

    for (int i = 0; i < extraUnbreakable; ++i) {
        int row, col;
        do {
            row = std::uniform_int_distribution<>(0, rows - 1)(rng);
            col = std::uniform_int_distribution<>(0, cols - 1)(rng);
        } while (usedPositions.count({row, col}));
        usedPositions.insert({row, col});

        float x = col * blockSizeX + 50;
        float y = row * blockSizeY + 50;
        blocks.emplace_back(x, y, BlockType::UNBREAKABLE);
    }

    std::uniform_int_distribution<> distBonusCount(5, 8);
    int bonusCount = distBonusCount(rng) - 1; // один уже добавлен выше

    for (int i = 0; i < bonusCount; ++i) {
        int row, col;
        do {
            row = std::uniform_int_distribution<>(0, rows - 1)(rng);
            col = std::uniform_int_distribution<>(0, cols - 1)(rng);
        } while (usedPositions.count({row, col}));
        usedPositions.insert({row, col});

        float x = col * blockSizeX + 50;
        float y = row * blockSizeY + 50;
        blocks.emplace_back(x, y, BlockType::BONUS);
    }

    std::uniform_int_distribution<> distFastBall(3, 5);
    int fastBallCount = distFastBall(rng);

    for (int i = 0; i < fastBallCount; ++i) {
        int row, col;
        do {
            row = std::uniform_int_distribution<>(0, rows - 1)(rng);
            col = std::uniform_int_distribution<>(0, cols - 1)(rng);
        } while (usedPositions.count({row, col}));
        usedPositions.insert({row, col});

        float x = col * blockSizeX + 50;
        float y = row * blockSizeY + 50;
        blocks.emplace_back(x, y, BlockType::FAST_BALL);
    }

    std::uniform_int_distribution<> distMultiHit(3, 5);
    int multiHitCount = distMultiHit(rng);

    for (int i = 0; i < multiHitCount; ++i) {
        int row, col;
        do {
            row = std::uniform_int_distribution<>(0, rows - 1)(rng);
            col = std::uniform_int_distribution<>(0, cols - 1)(rng);
        } while (usedPositions.count({row, col}));
        usedPositions.insert({row, col});

        float x = col * blockSizeX + 50;
        float y = row * blockSizeY + 50;
        blocks.emplace_back(x, y, BlockType::MULTIHIT);
    }

    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            if (!usedPositions.count({row, col})) {
                float x = col * blockSizeX + 50;
                float y = row * blockSizeY + 50;
                blocks.emplace_back(x, y, BlockType::NORMAL);
            }
        }
    }
}

void Game::checkCollisions() {
    for (auto it = blocks.begin(); it != blocks.end();) {
        Block& block = *it;

        // Коллизия с блоками
        if (!block.isDestroyed()) {
            if (ball.getGlobalBounds().intersects(block.getGlobalBounds())) {
                auto dir = getCollisionDirection(ball.getGlobalBounds(), block.getGlobalBounds());

                if (dir == CollisionDirection::LEFT || dir == CollisionDirection::RIGHT)
                    ball.reverseVelocityX();
                else if (dir == CollisionDirection::TOP || dir == CollisionDirection::BOTTOM)
                    ball.reverseVelocityY();

                block.hit();
                score += 1;
                scoreText.setString("Score: " + std::to_string(score));

                if (block.getType() == BlockType::BONUS) {
                    spawnBonus(block.getPosition());
                }

                if (block.getType() == BlockType::FAST_BALL){
                    ball.increaseSpeed();
                }
            }
        }

        if (block.isDestroyed()) {
            it = blocks.erase(it);
        } else {
            ++it;
        }
    }

    // Коллизия с кареткой
    if (ball.getGlobalBounds().intersects(paddle.getGlobalBounds())) {
        float ballCenterX = ball.getPosition().x + ball.getRadius();
        float paddleLeft = paddle.getPosition().x;

        float relativeIntersectX = ballCenterX - paddleLeft;
        float intersectionPercent = relativeIntersectX / paddle.getSize().x;

        ball.setVelocityX(-5.0f + 10.0f * intersectionPercent);

        // Пытаемся не застрять в каретке
        ball.setPosition(ball.getPosition().x, paddle.getPosition().y - ball.getRadius() * 2);

        ball.reverseVelocityY();
    }

    // Бонусы
    for (auto it = bonuses.begin(); it != bonuses.end();) {
        Bonus& bonus = *it;

        if (bonus.isActive() && bonus.getGlobalBounds().intersects(paddle.getGlobalBounds())) {
            applyBonus(bonus.getType());
            it = bonuses.erase(it);
        } else {
            bonus.update();
            if (!bonus.isActive()) {
                it = bonuses.erase(it);
            } else {
                ++it;
            }
        }
    }
}

void Game::spawnBonus(const sf::Vector2f& pos) {
    std::mt19937 rng(std::random_device{}());

    int bonusCount = static_cast<int>(BonusType::RANDOM_TRAJECTORY) + 1;

    int randomIndex = std::uniform_int_distribution<>(0, bonusCount - 1)(rng);

    auto type = static_cast<BonusType>(randomIndex);
    bonuses.emplace_back(pos.x, pos.y, type);
}

void Game::applyBonus(BonusType type) {
    switch (type) {
        case BonusType::WIDER_PADDLE:
            paddle.widen();
            break;
        case BonusType::FASTER_BALL:
            ball.increaseSpeed();
            break;
        case BonusType::SLOWER_BALL:
            ball.decreaseSpeed();
            break;
        case BonusType::STICKY:
            ball.setSticky(true);
            break;
        case BonusType::EXTRA_LIFE:
            lives++;
            livesText.setString("Lives: " + std::to_string(lives));
            break;
        case BonusType::BOTTOM_BONUS:
            hasBottomBonus = true;

            bottomBonusLine = sf::RectangleShape(sf::Vector2f(800, 5));
            bottomBonusLine.setPosition(0, 595);
            bottomBonusLine.setFillColor(sf::Color::Transparent);
            break;
        case BonusType::RANDOM_TRAJECTORY:
            ball.applyRandomTrajectory();
            break;
    }
}