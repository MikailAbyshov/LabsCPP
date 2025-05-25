#pragma once

#include <SFML/Graphics.hpp>

enum class BlockType {
    NORMAL,
    UNBREAKABLE,
    BONUS,
    FAST_BALL,
    MULTIHIT
};

class Block : public sf::RectangleShape {
public:
    Block(float x, float y, BlockType type);

    void hit();
    bool isDestroyed() const;
    BlockType getType() const;

private:
    BlockType type;
    int health = 1;
    bool destroyed = false;

    sf::Color getColorByType();
};