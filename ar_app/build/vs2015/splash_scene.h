#ifndef SPLASH_SCENE_H_
#define SPLASH_SCENE_H_

#include "base_scene.h"

#include <sony_sample_framework.h>

#include "graphics\font.h"
#include "graphics\sprite.h"


namespace hovar
{
	class SplashScene : public BaseScene
	{
	public:
		SplashScene();
		virtual ~SplashScene();

		void Init(gef::Platform* platform);
		bool Update(const float frame_time);
		void Render();
		void CleanUp();

	private:
		void InitFont();
		void DrawHUD();
		void RenderOverlay();

		gef::SpriteRenderer* sprite_renderer_;
		gef::Font* font_;
	};
}

#endif

