#ifndef _BASE_SCENE_H_
#define _BASE_SCENE_H_

#include "input\input_manager.h"

namespace hovar
{
	class BaseScene
	{
	public:
		BaseScene();
		virtual ~BaseScene();
		virtual void Init(gef::Platform *platform);
		virtual void CleanUp();
		virtual bool Update(const float frame_time);
		virtual void Render();


	protected:
		gef::Platform* platform_;
		gef::InputManager* input_manager_;
		float fps_;
	};
}


#endif _BASE_SCENE_H_

