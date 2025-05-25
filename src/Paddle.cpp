#include "../includes/Paddle.h"

Paddle::Paddle(float x, float y)
        : RectangleShape(sf::Vector2f(100.f, 20.f)) {
    setFillColor(sf::Color::Blue);
    setPosition(x, y);
}

void Paddle::update() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && getPosition().x > 0) {
        move(-speed, 0);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) &&
        getPosition().x + getSize().x < 800) {
        move(speed, 0);
    }
}

void Paddle::widen() {
    setSize(sf::Vector2f(getSize().x + 40, getSize().y));
}