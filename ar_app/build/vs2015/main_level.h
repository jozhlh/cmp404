#pragma once
#include "base_scene.h"
class MainLevel : public BaseScene
{
public:
	MainLevel();
	~MainLevel();

	void Init(gef::Platform& platform_);

private:


	float kGridSize;
};

