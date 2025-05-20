#pragma once
#include <algorithm>
#include <functional>

class Animation {
protected:
	float duration_;
	float elapsed_ = 0.f;
	bool finished_ = false;
	std::function<void()> onCompleteCallback_;

public:

	Animation(float duration) : duration_(duration) {}
	virtual ~Animation() = default;

	void update(float dt);

	virtual void onUpdate(float t) = 0;

	virtual void onComplete();

	bool isFinished() const;

	void setOnComplete(std::function<void()>);
};