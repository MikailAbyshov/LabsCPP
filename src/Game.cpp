#include "../includes/Game.h"
#include <random>
#include <iostream>
#include <filesystem>
#include <set>

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

        if (!stickyActivated) {
            ball.update();
        } else {
            float newX = paddle.getPosition().x + paddle.getSize().x / 2.f - ball.getRadius();
            float newY = paddle.getPosition().y - ball.getRadius() * 2;
            ball.setPosition(newX, newY);

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
                ball.reverseVelocityY();
                stickyActivated = false;
                ballReleased = true;
            }
        }

        // Проверка выхода за нижнюю границу
        if (ball.getPosition().y > window.getSize().y) {
            if (hasBottomBonus) {
                ball.reverseVelocityY();
                ball.setPosition(ball.getPosition().x, bottomLineY - ball.getRadius() * 2);
                hasBottomBonus = false;
            } else {
                lives--;
                livesText.setString("Lives: " + std::to_string(lives));
                ball.reset();
                ballReleased = true;
                if (lives <= 0) window.close();
            }
        }

        checkCollisions();

        // Отрисовка
        window.clear();
        window.draw(paddle);
        window.draw(ball);
        for (auto& block : blocks) window.draw(block);
        for (auto& bonus : bonuses) window.draw(bonus);
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

    // Все возможные позиции на поле
    std::vector<std::pair<int, int>> allPositions;
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            allPositions.emplace_back(row, col);
        }
    }

    // Перемешиваем их, чтобы случайно распределить блоки
    std::shuffle(allPositions.begin(), allPositions.end(), rng);

    // Типы специальных блоков
    std::vector<BlockType> effectBlockTypes = {
        BlockType::UNBREAKABLE,
        BlockType::BONUS,
        BlockType::FAST_BALL,
        BlockType::MULTIHIT
    };

    // Счётчики для каждого типа
    std::map<BlockType, int> blockCounts;

    // Устанавливаем случайное количество от 1 до 5 для каждого типа
    std::uniform_int_distribution<> countDist(1, 5);
    for (const auto& type : effectBlockTypes) {
        blockCounts[type] = countDist(rng);
    }

    // Заполняем блоки
    blocks.clear();
    std::set<std::pair<int, int>> usedPositions;

    size_t posIndex = 0;

    // Сначала ставим все специальные блоки
    for (const auto& [type, count] : blockCounts) {
        for (int i = 0; i < count && posIndex < allPositions.size(); ++i) {
            auto [row, col] = allPositions[posIndex++];
            float x = col * blockSizeX + 50;
            float y = row * blockSizeY + 50;
            blocks.emplace_back(x, y, type);
        }
    }

    // Оставшиеся позиции заполняем обычными блоками
    while (posIndex < allPositions.size()) {
        auto [row, col] = allPositions[posIndex++];
        float x = col * blockSizeX + 50;
        float y = row * blockSizeY + 50;
        blocks.emplace_back(x, y, BlockType::NORMAL);
    }
}

void Game::checkCollisions() {
    for (auto it = blocks.begin(); it != blocks.end();) {
        Block& block = *it;

        // Коллизия с блоками
        if (!block.isDestroyed()) {
            if (ball.getGlobalBounds().intersects(block.getGlobalBounds())) {
                ball.reverseVelocityX();
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
        ball.setPosition(ball.getPosition().x, paddle.getPosition().y - ball.getRadius() * 2);

        if (ball.getIsSticky()) {
            stickyActivated = true;
            ball.setSticky(false);
        } else {
            ball.reverseVelocityY();
        }
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