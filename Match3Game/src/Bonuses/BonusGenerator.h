#pragma once
#include "IBonusStrategy.h"
#include <vector>
#include <memory>
#include <random>
#include <algorithm>
#include <optional>

class Pos;

class BonusGenerator {
	std::vector<std::unique_ptr<IBonusStrategy>> strategies_;
	std::mt19937 rng_;
public:
	BonusGenerator(std::mt19937& rng);

	void addStrategy(std::unique_ptr <IBonusStrategy> strat);

	std::optional<BonusEvent> generate(Board& board, const Pos& pos);
};