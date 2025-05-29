#include "../includes/Block.h"

Block::Block(float x, float y, BlockType type)
        : RectangleShape(sf::Vector2f(70.f, 30.f)), type(type), destroyed(false) {
    setPosition(x, y);

    switch (type) {
        case BlockType::NORMAL:
            health = 1;
            break;
        case BlockType::UNBREAKABLE:
            health = -1;
            break;
        case BlockType::BONUS:
            health = 1;
            break;
        case BlockType::FAST_BALL:
            health = 1;
            break;
        case BlockType::MULTIHIT:
            health = 3;
            break;
    }

    setFillColor(getColor());
}

void Block::hit() {
    if (type == BlockType::UNBREAKABLE) return;

    health--;
    if (health <= 0) {
        destroyed = true;
        setFillColor(sf::Color::Black);
    }
}

bool Block::isDestroyed() const {
    return destroyed;
}


BlockType Block::getType() const {
    return type;
}

sf::Color Block::getColor() {
    switch (type) {
        case BlockType::NORMAL: return sf::Color::Green;
        case BlockType::UNBREAKABLE: return sf::Color::White;
        case BlockType::BONUS: return sf::Color::Yellow;
        case BlockType::FAST_BALL: return sf::Color::Magenta;
        case BlockType::MULTIHIT: return sf::Color::Cyan;
        default: return sf::Color::White;
    }
}
