#ifndef SPLASH_SCENE_H_
#define SPLASH_SCENE_H_

#include "base_scene.h"

#include <sony_sample_framework.h>

#include "graphics\font.h"
#include "graphics\sprite.h"


namespace hovar
{
	/**
	First scene to be loaded.
	*/
	class SplashScene : public BaseScene
	{
	public:
		/// @brief Default constructor.
		/// @note No default functionality.
		SplashScene();

		/// @brief Default destructor.
		/// @note No default functionality.
		virtual ~SplashScene();

		/// @brief Initialises main menu.
		/// @note Creates all objects in the scene.
		/// @param[in] platform		The platform object.
		void Init(gef::Platform* platform);

		/// @brief Main menu loop.
		/// @note Waits for user to select an action.
		/// @param[in] frame_time	Time since the last update function (delta time).
		/// @return False if player wishes to advance to main level.
		bool Update(const float frame_time);

		/// @brief Renders scene.
		/// @note Renders main menu UI.
		void Render();

		/// @brief Releases unused memory.
		void CleanUp();

	private:
		/// @brief Loads font data.
		/// @note Loads the comic sans font.
		/// http://explosm.net/comics/2301/
		void InitFont();

		/// @brief Draw UI components.
		/// @note Draws the title and instructions to the player.
		void DrawHUD();

		/// @brief Render UI.
		void RenderOverlay();

		/// The 3D rendering engine.
		gef::SpriteRenderer* sprite_renderer_;

		/// The font data.
		gef::Font* font_;
	};
}

#endif

