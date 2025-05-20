#pragma once
#include <SFML/Graphics.hpp>
#include <random>
#include "Pos.h"


enum class TileType { Red, Green, Blue, Yellow, Purple, EMPTY };

class Tile {

  public:
    Tile() = default;
    Tile(const Tile&) = default;
    Tile(const Pos& pos, TileType type, float tileSize);

        sf::Vector2f getPxPosition() const;

    void setScale(float x, float y);

    void setLogicalPos(const Pos& pos);

    Pos getLogicalPos() const;

    void setBonus(char c);
    void clearBonus();

    void setType(TileType type);

    TileType getType() const;

    void draw(sf::RenderWindow& window);

    static TileType randomColor(std::mt19937& rng);

    void setPxPosition(const sf::Vector2f& pixelPos);

  private:
    Pos pos_;
    TileType type_;
    float size_;
    sf::RectangleShape shape_;

    bool hasBonus_ = false;
    char bonusChar_;

    void setPositionByPosAndSize(const Pos& pos, float tileSize);
};
