#pragma once
#include "IBonusStrategy.h"
#include <random>


class PainterBonusStrategy : public IBonusStrategy {
	std::uniform_real_distribution<float> dist_{ 0.f, 1.f };
	float chance_;
	static int const actionRad = 3;
	static int const selelectionNum = 2;
public:
	PainterBonusStrategy(float chance = 0.50f);
	Bonus tryGenerate(const Pos&, std::mt19937& rng) override;
	void apply(Board& board, const Pos& originPos, const Pos& originBonusPos) override;
};