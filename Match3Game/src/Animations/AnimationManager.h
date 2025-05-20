#pragma once
#include "Animation.h"
#include <vector>
#include <memory>
#include <SFML/System/Vector2.hpp>


class AnimationManager {
private:
	std::vector<std::unique_ptr<Animation>> animations_;
public:
	void add(std::unique_ptr<Animation> animation);

	void updateAll(float dt);

	bool empty() const;
};