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
    void applyRandomTrajectory();
    float getRadius() const { return radius; }
    bool getIsSticky() const {return isSticky; }

private:
    float radius;
    bool isSticky = false;
    const float МaxSpeed = 5.0f;
    const float MinSpeed = 1.5f;

    sf::Vector2f velocity;
};
