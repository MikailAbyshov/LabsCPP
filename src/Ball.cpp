#include <cmath>
#include "../includes/Ball.h"

Ball::Ball(float x, float y)
        : CircleShape(10.f), radius(10.f) {
    setFillColor(sf::Color::Red);
    setPosition(x, y);
    velocity = {3.f, -3.f};
}

void Ball::update() {
    sf::Vector2f pos = getPosition();
    if (pos.x <= 0 || pos.x + 20 >= 800) reverseVelocityX();
    if (pos.y <= 0) reverseVelocityY();

    move(velocity);
}

void Ball::applyRandomTrajectory() {
    float speed = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
    float angle = static_cast<float>(rand() % 360) * 3.14159265f / 180.f;

    velocity.x = speed * std::cos(angle);
    velocity.y = speed * std::sin(angle);
}

void Ball::reverseVelocityY() {
    velocity.y = -velocity.y;
}

void Ball::reverseVelocityX() {
    velocity.x = -velocity.x;
}

void Ball::reset() {
    setPosition(400.f, 300.f);
    velocity = {3.f, -3.f};
    isSticky = false;
}

void Ball::increaseSpeed() {
    const float maxSpeed = 5.0f;
    velocity.x = std::min(velocity.x + 1.5f, maxSpeed);
    velocity.y = std::min(velocity.y + 1.5f, maxSpeed);
}

void Ball::decreaseSpeed() {
    const float minSpeed = 1.5f;
    velocity.x = std::max(velocity.x - 0.5f, minSpeed);
    velocity.y = std::max(velocity.y - 0.5f, minSpeed);
}

void Ball::setSticky(bool sticky) {
    isSticky = sticky;
}

void Ball::setVelocityX(float vx) {
    velocity.x = vx;
}