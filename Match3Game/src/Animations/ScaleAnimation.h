#pragma once
#include "Animation.h"
#include <SFML/System/Vector2.hpp>

template<typename T>
class ScaleAnimation : public Animation {
private:
	T& target_;
	float fromScale_, toScale_;

public:
	ScaleAnimation(T& target, float fromScale, 
		float toScale, float duration)
		: Animation(duration), target_(target),
		fromScale_(fromScale), toScale_(toScale) {}

	void onUpdate(float t) override {
		float scale = fromScale_ + (toScale_ - fromScale_) * t;
		target_.setScale(scale, scale);
	}

	void onComplete() override {
		target_.setScale(toScale_, toScale_);
	}
};
