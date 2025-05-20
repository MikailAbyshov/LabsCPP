#pragma once
#include "Animation.h"
#include <SFML/System/Vector2.hpp>

template<typename T>
class MoveAnimation : public Animation {
private:
	T& target_;
	sf::Vector2f from_, to_;

public:
	MoveAnimation(T& target, sf::Vector2f from, 
		sf::Vector2f to, float duration)
		: Animation(duration), target_(target), from_(from), to_(to) {}

	void onUpdate(float t) override {
		sf::Vector2f pos = from_ + (to_ - from_) * t;
		target_.setPxPosition(pos);
	}

	void onComplete() override {
		target_.setPxPosition(to_);
	}
};