#include "AnimationManager.h"
#include <algorithm>
#include <iostream>

void AnimationManager::add(std::unique_ptr<Animation> animation) {
	animations_.push_back(std::move(animation));
}

void AnimationManager::updateAll(float dt) {
	for (auto& anim : animations_) {
		anim->update(dt);
	}
	animations_.erase(
		std::remove_if(
			animations_.begin(),
			animations_.end(),
			[](const std::unique_ptr<Animation>& anim) {
				return anim->isFinished();
			}
		),
		animations_.end()
	);
}

bool AnimationManager::empty() const {
	return animations_.empty();
}