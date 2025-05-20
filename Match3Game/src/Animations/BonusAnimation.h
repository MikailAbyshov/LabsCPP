#pragma once
#include "Animation.h"
#include "../Pos.h"
#include "../Board.h"
#include <SFML/Graphics.hpp>

class BonusAnimation : public Animation {
public:
    BonusAnimation(
        sf::RenderWindow& window,
        std::function<sf::Vector2f(const Pos&)> cellCenter,
        const sf::Font& font,
        Pos pos, char c, float duration);

    void onUpdate(float t) override;
    void onComplete() override;

private:
    sf::RenderWindow& window_;
    std::function<sf::Vector2f(const Pos&)> cellCenter_;
    sf::Text text_;
};
