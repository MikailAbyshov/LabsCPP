#include "BonusAnimation.h"

BonusAnimation::BonusAnimation(
    sf::RenderWindow& window,
    std::function<sf::Vector2f(const Pos&)> cellCenter,
    const sf::Font& font,
    Pos pos, char c, float duration) 
    : Animation(duration), window_(window), cellCenter_(cellCenter)
{
    text_.setFont(font);
    text_.setString(std::string(1, c));
    text_.setCharacterSize((unsigned)(window.getSize().y / Board::ROWS * 0.8f));
    text_.setOrigin(
        text_.getLocalBounds().width / 2.f,
        text_.getLocalBounds().height / 2.f + window.getSize().y/30);
    sf::Color col = text_.getFillColor();
    col.a = 0;
    text_.setFillColor(col);
    text_.setPosition(cellCenter_(pos));
}

void BonusAnimation::onUpdate(float t) {
    float alpha;
    if (t < 0.3f) {
        alpha = (t / 0.3f) * 255.f;
    }
    else if (t < 0.5f) {
        alpha = 255.f;
    }
    else {
        alpha = ((0.7f - t) / 0.2f) * 255.f;
    }
    sf::Color col = text_.getFillColor();
    col.a = static_cast<sf::Uint8>(std::clamp(alpha, 0.f, 255.f));
    text_.setFillColor(col);

    window_.draw(text_);
}

void BonusAnimation::onComplete() {
    sf::Color col = text_.getFillColor();
    col.a = 0;
    text_.setFillColor(col);
}
