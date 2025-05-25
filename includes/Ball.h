#pragma once

#include <SFML/Graphics.hpp>

class Ball : public sf::CircleShape {
public:
    explicit Ball(float x, float y);

    void update();
    void reverseVelocityY();
    void reverseVelocityX();
    void reset();
    void increaseSpeed();
    void decreaseSpeed();
    void setSticky(bool sticky);
    void setVelocityX(float vx);

    float getRadius() const { return radius; }

private:
    float velocityX = 3.f;
    float velocityY = -3.f;
    float radius;
    bool isSticky = false;
    sf::Vector2f velocity;
};
