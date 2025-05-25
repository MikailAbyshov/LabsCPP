#pragma once

#include <SFML/Graphics.hpp>

class Paddle : public sf::RectangleShape {
public:
    explicit Paddle(float x, float y);

    void update();
    void widen();

private:
    float speed = 6.0f;
};