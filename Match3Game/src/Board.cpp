#include "Board.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include "Bonuses/BombBonusStrategy.h"
#include "Bonuses/PainterBonusStrategy.h"
#include <iostream>


Board::Board() : rng_(static_cast<unsigned>(std::time(nullptr))), bonusGen_(rng_)
{
	bonusGen_.addStrategy(std::make_unique<BombBonusStrategy>(0.03f));
	bonusGen_.addStrategy(std::make_unique<PainterBonusStrategy>(0.03f));


	stratMap_.emplace(Bonus::Bomb, std::make_unique<BombBonusStrategy>());
	stratMap_.emplace(Bonus::Painter, std::make_unique<PainterBonusStrategy>());
	reset();
}

void Board::reset() {
	std::uniform_int_distribution<int> dist(0, 4);
	grid_.assign(ROWS, std::vector<Tile>(COLS, Tile({0 , 0}, TileType::EMPTY, tileSize_)));
	for (int row = 0; row < ROWS; row++) {
		for (int col = 0; col < COLS; col++) {
			TileType t = static_cast<TileType>(dist(rng_));
			grid_[row][col] = Tile({row, col}, t, tileSize_);
			grid_[row][col].setPxPosition(cellCenterPx({ row, col }));
		}
	}
}


void Board::draw(sf::RenderWindow& window) {
	for (auto& row : grid_) {
		for (auto& tile : row) {
			tile.draw(window);
		}
	}
}

std::optional<std::pair<Pos, Pos>> Board::select(const Pos& position) {
	if (!firstSelection_) {
		firstSelection_ = position;
		return std::nullopt;
	}
	Pos aPos = firstSelection_.value(), bPos = position;
	firstSelection_.reset();
	if (isAdjacent(aPos, bPos)) {
		return std::make_pair(aPos, bPos);
	}
	firstSelection_ = bPos;
	return std::nullopt;
}

bool Board::isAdjacent(const Pos& aPos, const Pos& bPos) {
	return std::abs(aPos.row - bPos.row) + std::abs(aPos.col - bPos.col) == 1;
}


bool Board::swapTiles(const Pos& aPos, const Pos& bPos) {
	std::swap(grid_[aPos.row][aPos.col], grid_[bPos.row][bPos.col]);
	grid_[aPos.row][aPos.col].setLogicalPos(aPos);
	grid_[bPos.row][bPos.col].setLogicalPos(bPos);
	return true;
}

std::set<Pos> Board::findMatches() {
	std::set<Pos> result;

	for (int row = 0; row < ROWS; row++) {
		for (int col = 0; col < COLS - 2; col++) {
			auto tileType = grid_[row][col].getType();
			if (tileType != TileType::EMPTY &&
				grid_[row][col + 1].getType() == tileType &&
				grid_[row][col + 2].getType() == tileType) 
			{
				int nextCol = col;
				while (nextCol < COLS && grid_[row][nextCol].getType() == tileType) {
					result.insert({ row,nextCol });
					nextCol++;
				}
			}
		}
	}
	for (int col = 0; col < COLS; col++) {
		for (int row = 0; row < ROWS - 2; row++) {
			auto tileType = grid_[row][col].getType();
			if (tileType != TileType::EMPTY &&
				grid_[row + 1][col].getType() == tileType &&
				grid_[row + 2][col].getType() == tileType)
			{
				int nextRow = row;
				while (nextRow < ROWS && grid_[nextRow][col].getType() == tileType) {
					result.insert({ nextRow,col });
					nextRow++;
				}
			}
		}
	}
	return result;
}

void Board::removeMatched(const std::set<Pos>& metchedPoses) {
	for (auto [row, col] : metchedPoses) {
		grid_[row][col].setType(TileType::EMPTY);
	}
	isAnimationsRunning_ = true;
}

std::vector<TileMove> Board::computeCollapseMoves()
{
	std::cout << "Before collapse:\n";
	for (int r = 0; r < ROWS; ++r) {
		for (int c = 0; c < COLS; ++c) std::cout << int(grid_[r][c].getType()) << ' ';
		std::cout << '\n';
	}

	std::vector<TileMove> moves;
	for (int col = 0; col < COLS; ++col) {
		int writeRow = ROWS - 1;
		for (int row = ROWS - 1; row >= 0; --row) {
			if (grid_[row][col].getType() != TileType::EMPTY) {
				if (row != writeRow) {
					moves.push_back({ &grid_[row][col],
									  {row, col},
									  {writeRow, col} });
				}
				--writeRow;
			}
		}
		for (int row = writeRow; row >= 0; --row) {
			moves.push_back({ nullptr, {-1, col}, {row, col} });
		}
	}
	return moves;
}

void Board::applyCollapseMoves(std::vector<TileMove> const& moves)
{

	std::uniform_int_distribution<int> dist(0, 4);

	for (auto const& m : moves) {
		if (m.tile) {
			Tile moved = std::move(*m.tile);
			moved.setLogicalPos(m.to);
			grid_[m.to.row][m.to.col] = std::move(moved);
			grid_[m.from.row][m.from.col].setType(TileType::EMPTY);
			grid_[m.from.row][m.from.col].clearBonus();
		}
	}

	for (auto const& m : moves) {
		if (!m.tile) {
			Tile newTile(m.to, static_cast<TileType>(dist(rng_)), tileSize_);
			newTile.setPxPosition(cellCenterPx(m.from)); // старт выше
			grid_[m.to.row][m.to.col] = std::move(newTile);
		}
	}

	for (int r = 0; r < ROWS; ++r) {
		for (int c = 0; c < COLS; ++c) {
			if (grid_[r][c].getType() == TileType::EMPTY) {
				Tile fallbackTile({ r, c }, static_cast<TileType>(dist(rng_)), tileSize_);
				fallbackTile.setPxPosition(cellCenterPx({ r, c }));
				grid_[r][c] = std::move(fallbackTile);
				std::cout << "[FIX] Filled EMPTY at (" << r << "," << c << ")\n";
			}
		}
	}
	std::cout << "After collapse:\n";
	for (int r = 0; r < ROWS; ++r) {
		for (int c = 0; c < COLS; ++c) std::cout << int(grid_[r][c].getType()) << ' ';
		std::cout << '\n';
	}
}

std::vector<Pos> Board::getRandPosInRad(int selectionNum, int rad, const Pos& originPose) {
	std::vector<Pos> result;
	result.reserve(selectionNum);
	int count = 0;
	for (int drow = -rad; drow <= rad; ++drow) {
		for (int dcol = -rad; dcol <= rad; ++dcol) {
			int row = originPose.row + drow;
			int col = originPose.col + dcol;
			if (row < 0 || row >= ROWS || col < 0 || col >= COLS) continue;
			count++;
			if (result.size() < selectionNum) {
				result.push_back({ row,col });
			}
			else {
				int idx = std::uniform_int(0, count - 1)(rng_);
				if (idx < selectionNum) {
					result[idx] = { row, col };
				}
			}
		}
	}
	return result;
}

std::vector<Pos> Board::getRandPosBetweenRad(int selectionNum, int OutRad, int innerRad, const Pos& originPose) {
	std::vector<Pos> result;
	result.reserve(selectionNum);
	int count = 0;
	for (int drow = -OutRad; drow <= OutRad; ++drow) {
		for (int dcol = -OutRad; dcol <= OutRad; ++dcol) {
			if (abs(drow) <= innerRad || abs(dcol) <= innerRad) { continue; }
			int row = originPose.row + drow;
			int col = originPose.col + dcol;
			if (row < 0 || row >= ROWS || col < 0 || col >= COLS) { continue; }
			count++;
			if (result.size() < count) {
				result.push_back({ row, col });
			}
			else {
				int idx = std::uniform_int(0, count - 1)(rng_);
				if (idx < selectionNum) {
					result[idx] = { row, col };
				}
			}
		}
	}
	return result;
}

void Board::addInstantBonus(const std::set<Pos>& metchedPoses) {
	for (auto& pos : metchedPoses) {
		auto bonus = bonusGen_.generate(*this, pos);
		if (bonus.has_value()  && bonusCb_) {
			std::cout << "bonus callback\n";
			bonusCb_(bonus.value());
		}
	}
}

void Board::removeSelected(const std::vector<Pos>& selectedPoses) {
	for (auto& pos : selectedPoses) {
		grid_[pos.row][pos.col].setType(TileType::EMPTY);
	}
}

void Board::setBonusCallback(BonusCallback cb) {
	bonusCb_ = std::move(cb);
}

void Board::applyBonus(BonusEvent& bonus) {
	auto strategy = stratMap_.find(bonus.type);
	if (strategy == stratMap_.end()) { return; }
	strategy->second->apply(*this, bonus.pos, bonus.posBonus);

}

std::mt19937& Board::getRng() {
	return rng_;
}

void Board::repaintByPos(TileType color, const Pos& pos) {
	std::cout << pos.row << "," << pos.col << " -> \n";
	grid_[pos.row][pos.col].setType(color);
}


Tile& Board::getTileByPos(const Pos& pos) {
	return grid_[pos.row][pos.col];
}

sf::Vector2f Board::cellCenterPx(const Pos& pos) const
{
	if (pos.row == -1) {
		return { pos.col * tileSize_ + tileSize_ / 2, -tileSize_ };
	}
	float x = pos.col * tileSize_ + tileSize_ * 0.5f;
	float y = pos.row * tileSize_ + tileSize_ * 0.5f;
	return { x, y };
}

float Board::getTileSize() const {
	return tileSize_;
}

void Board::syncTilesToLogical() {
	for (int r = 0; r < ROWS; ++r)
		for (int c = 0; c < COLS; ++c) {
			Tile& tile = grid_[r][c];
			tile.setLogicalPos({ r, c });
			tile.setPxPosition(cellCenterPx({ r, c }));
		}
}


