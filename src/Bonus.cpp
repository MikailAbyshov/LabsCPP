#include "../includes/Bonus.h"

Bonus::Bonus(float x, float y, BonusType t)
        : RectangleShape(sf::Vector2f(20.f, 20.f)), type(t) {
    setPosition(x, y);
    setFillColor(getColorByType());
}

BonusType Bonus::getType() const {
    return type;
}

void Bonus::setActive(bool active) {
    this->active = active;
}

bool Bonus::isActive() const {
    return active;
}

void Bonus::update() {
    move(0, 2);
    if (getPosition().y > 600) active = false;
}

sf::Color Bonus::getColorByType() {
    switch (type) {
        case BonusType::WIDER_PADDLE: return sf::Color::Blue;
        case BonusType::FASTER_BALL: return sf::Color::Red;
        case BonusType::SLOWER_BALL: return sf::Color::Green;
        case BonusType::STICKY: return sf::Color::Yellow;
        case BonusType::EXTRA_LIFE: return sf::Color::Magenta;
        case BonusType::BOTTOM_BONUS: return sf::Color(255, 165, 0);
        case BonusType::RANDOM_TRAJECTORY: return sf::Color::Cyan;
        default: return sf::Color::White;
    }
}
