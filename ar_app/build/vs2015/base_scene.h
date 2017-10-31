#ifndef _BASE_SCENE_H_
#define _BASE_SCENE_H_

#include "input\input_manager.h"

class BaseScene
{
public:
	BaseScene();
	virtual ~BaseScene();
	virtual void Init(gef::Platform& platform_);
	virtual void CleanUp();
	virtual bool Update(const float frame_time);
	virtual void Render();


protected:
	gef::InputManager* input_manager_;
	float fps_;
};

#endif _BASE_SCENE_H_

