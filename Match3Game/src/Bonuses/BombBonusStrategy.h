#pragma once
#include "IBonusStrategy.h"
#include <random>


class BombBonusStrategy : public IBonusStrategy {
    std::uniform_real_distribution<float> dist_{ 0.f, 1.f };
    float chance_;
    static int const  destoyedTileNum_ = 5;
public:
    BombBonusStrategy(float chance = 0.50f);
    Bonus tryGenerate(const Pos&, std::mt19937& rng) override;
    void apply(Board& board, const Pos& originPos, const Pos& originBonusPos) override;
};