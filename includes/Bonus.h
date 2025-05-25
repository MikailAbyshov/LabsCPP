#pragma once

#include <SFML/Graphics.hpp>

enum class BonusType {
    WIDER_PADDLE,
    FASTER_BALL,
    SLOWER_BALL,
    STICKY,
    EXTRA_LIFE
};

class Bonus : public sf::RectangleShape {
public:
    Bonus(float x, float y, BonusType type);

    BonusType getType() const;
    void setActive(bool active);
    bool isActive() const;

    void update();

private:
    BonusType type;
    bool active = true;
    sf::Color getColorByType();
};
