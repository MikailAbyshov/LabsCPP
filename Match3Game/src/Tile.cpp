#include "Tile.h"
#include <map>
#include <random>

Tile::Tile(const Pos& pos, TileType type, float tileSize)
    : pos_(pos), type_(type), size_(tileSize)
{
    shape_.setSize({ tileSize - 2.f, tileSize - 2.f });
    shape_.setOrigin(tileSize / 2.f, tileSize / 2.f);
    setType(type_);
    setPositionByPosAndSize(pos, tileSize);
    shape_.setOutlineThickness(1.f);
    shape_.setOutlineColor(sf::Color::White);
}

void Tile::setPositionByPosAndSize(const Pos& pos, float tileSize)
{
    pos_ = pos;
    sf::Vector2f current_px = { pos.col * tileSize + tileSize / 2.f,
        pos.row * tileSize + tileSize / 2.f };
    shape_.setPosition(current_px);
}

void Tile::setType(TileType type)
{ 
    type_ = type;
    static const std::map<TileType, sf::Color> colors = {
        {TileType::Red, sf::Color::Red}, {TileType::Green, sf::Color::Green},
        {TileType::Blue, sf::Color::Blue},
        {TileType::Yellow, sf::Color::Yellow},
        {TileType::Purple, sf::Color(128, 0, 128)},
        {TileType::EMPTY, sf::Color::Transparent}};
    shape_.setFillColor(colors.at(type_));
}

TileType Tile::getType() const { return type_; }

void Tile::draw(sf::RenderWindow& window)
{
    if (type_ == TileType::EMPTY)
        return;
    window.draw(shape_);
    if (hasBonus_) {
        sf::Text txt;
        txt.setString(std::string(1, bonusChar_));
        txt.setPosition(shape_.getPosition());
        window.draw(txt);
    }
}

TileType Tile::randomColor(std::mt19937& rng) {
    constexpr int ColorCount = static_cast<int>(TileType::EMPTY);

    std::uniform_int_distribution<int> dist(0, ColorCount - 1);
    return static_cast<TileType>(dist(rng));
}

void Tile::setPxPosition(const sf::Vector2f& pixelPos) {
    shape_.setPosition(pixelPos);
}

sf::Vector2f Tile::getPxPosition() const {
    return shape_.getPosition();
}

void Tile::setScale(float x, float y) {
    shape_.setScale(x, y);
}

void Tile::setLogicalPos(const Pos& pos) {
    pos_ = pos;
}

Pos Tile::getLogicalPos() const {
    return pos_;
}

void Tile::setBonus(char c) { hasBonus_ = true; bonusChar_ = c; }
void Tile::clearBonus() { hasBonus_ = false; }