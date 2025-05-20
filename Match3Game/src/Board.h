#pragma once
#include "Tile.h"
#include "Pos.h"
#include "Bonuses/IBonusStrategy.h"
#include "Bonuses/BonusGenerator.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <optional>
#include <set>
#include <random>
#include <tuple>
#include <functional>
#include <array>

struct TileMove {
    Tile* tile;
    Pos   from;
    Pos   to;
};

struct PosHash {
    size_t operator()(const Pos& p) const {
        return p.row * 1000 + p.col;
    }
};

class Board {
public:
    using BonusCallback = std::function<void(const BonusEvent&)>;

    static const int ROWS = 8, COLS = 8;
    static constexpr float BONUS_PROBABILITY = 0.50f;

    using TileMap = std::unordered_map<Pos, Tile, PosHash>;

    Board();

    void reset();

    void draw(sf::RenderWindow&);

    std::optional<std::pair<Pos, Pos>> select(const Pos& position);

    std::vector<Pos> getRandPosInRad(int selectionNum, int rad, const Pos& originPose);

    std::vector<Pos> getRandPosBetweenRad(int selectionNum, int OutRad, int innerRad, const Pos& originPose);

    void removeSelected(const std::vector<Pos>& selectedPoses);

    void setBonusCallback(BonusCallback cb);

    void applyBonus(BonusEvent& bonus);

    void repaintByPos(TileType color, const Pos& pos);

    std::mt19937& getRng();

    std::set<Pos> findMatches();

    Tile& getTileByPos(const Pos& pos);

    bool swapTiles(const Pos& aPos, const Pos& bPos);

    void removeMatched(const std::set<Pos>& metchedPoses);

    void addInstantBonus(const std::set<Pos>& metchedPoses);

    std::vector<TileMove> computeCollapseMoves();

    void applyCollapseMoves(std::vector<TileMove> const& moves);

    sf::Vector2f cellCenterPx(const Pos& pos) const;

    float getTileSize() const;

    void syncTilesToLogical();

    TileMap getTileMap() const {
        TileMap map;
        for (int r = 0; r < ROWS; ++r)
            for (int c = 0; c < COLS; ++c)
                map[{r, c}] = grid_[r][c];
        return map;
    }

private:
    float tileSize_ = 50.f;

    bool isAnimationsRunning_ = false;

    std::vector<std::vector<Tile>> grid_;

    std::vector<Tile> spawnBuffer_;

    std::optional<Pos> firstSelection_;

    std::mt19937 rng_;

    BonusGenerator bonusGen_;

    BonusCallback bonusCb_;

    std::unordered_map<Bonus, std::unique_ptr<IBonusStrategy>, BonusHash> stratMap_;

    bool isAdjacent(const Pos& aPos, const Pos& bPos);
};
