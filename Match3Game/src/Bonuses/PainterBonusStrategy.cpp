#include "PainterBonusStrategy.h"
#include "../Board.h"
#include "../Pos.h"
#include <iostream>


PainterBonusStrategy::PainterBonusStrategy(float chance) : chance_(chance) {}

Bonus PainterBonusStrategy::tryGenerate(const Pos&, std::mt19937& rng) {
	return dist_(rng) < chance_ ? Bonus::Painter : Bonus::None;
}

void PainterBonusStrategy::apply(Board& board, const Pos& originPos, const Pos& originBonusPos) {
	std::cout << "Painter" << std::endl;
	auto selectedPoses = board.getRandPosBetweenRad(selelectionNum, actionRad, 1, originBonusPos);
	TileType newColor = board.getTileByPos(originPos).getType();
	for (auto& pos : selectedPoses) {
		std::cout << "color = " << static_cast<int>(newColor) << std::endl;
		board.repaintByPos(newColor, pos);
	}
	board.repaintByPos(newColor, originBonusPos);


}