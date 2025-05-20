#include "BombBonusStrategy.h"
#include "../Board.h"
#include <iostream>

BombBonusStrategy::BombBonusStrategy(float chance) :chance_(chance) {}

Bonus BombBonusStrategy::tryGenerate(const Pos&, std::mt19937& rng) {
    return dist_(rng) < chance_ ? Bonus::Bomb : Bonus::None;
}

void BombBonusStrategy::apply(Board& board, const Pos& originPos, const Pos& originBonusPos) {
    std::cout << "Bomb" << std::endl;
    std::vector<Pos> selectedPoses = board.getRandPosInRad(destoyedTileNum_, board.ROWS, originBonusPos);
    selectedPoses.push_back(originBonusPos);
    board.removeSelected(selectedPoses);
}