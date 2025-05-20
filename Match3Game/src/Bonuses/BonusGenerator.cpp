#include "BonusGenerator.h"
#include "../Board.h"
#include <optional>
#include <iostream>

BonusGenerator::BonusGenerator(std::mt19937& rng) : rng_(rng) {}

void BonusGenerator::addStrategy(std::unique_ptr <IBonusStrategy> strat) {
	BonusGenerator::strategies_.push_back(std::move(strat));
}

std::optional<BonusEvent> BonusGenerator::generate(Board& board, const Pos& pos) {
	Pos bonusPos = board.getRandPosInRad(1, 3, pos)[0];
	std::shuffle(strategies_.begin(), strategies_.end(), rng_);
	Bonus resultbonus = Bonus::None;
	for (auto& strat : strategies_) {
		Bonus bonus = strat->tryGenerate(bonusPos, rng_);
		if (bonus == Bonus::None)
			return std::nullopt;
		else {
			std::cout << "bonus not None\n";
			return BonusEvent{ bonus, pos, bonusPos };;
		}
	}

}