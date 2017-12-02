/*
	ar_app.h

	Application class which runs the hovar game.

	@author	Josh Hale 
	Last Edited: 02/12/17
*/

#ifndef _RENDER_TARGET_APP_H
#define _RENDER_TARGET_APP_H

#include <system/application.h>

#include "build\vs2015\base_scene.h"
#include "build\vs2015\main_level.h"
#include "build\vs2015\splash_scene.h"

// Vita AR includes
#include <camera.h>
#include <gxm.h>
#include <motion.h>
#include <libdbg.h>
#include <libsmart.h>

// FRAMEWORK FORWARD DECLARATIONS
namespace gef
{
	class Platform;
}

/**
Application class which runs the hovar game.
*/
class ARApp : public gef::Application
{
public:
	/// @brief Default constructor.
	/// @note Parent constructor is executed using the platform parameter.
	/// @param[in] platform		The platform object.
	ARApp(gef::Platform& platform);

	/// @brief Creates scenes and initialises splash screen.
	void Init();

	/// @brief Release unused memory.
	/// @note Releases the currently loaded scene to make way for the new scene.
	void CleanUp();

	/// @brief Update.
	/// @note The currrent scene's update is executed, if it returns false the next scene is loaded.
	/// @param[in] frame_time		The delta time (time since last frame).
	/// @return Always returns true.
	bool Update(float frame_time);

	/// @brief Render the current scene
	void Render();

private:
	/// @brief Load next scene.
	/// @note Releases the currently loaded scene and initialises the next scene.
	void SwitchScene();

	/// Currently loaded scene.
	hovar::BaseScene* current_scene_;

	/// The main level scene.
	hovar::MainLevel* main_scene_;

	/// The entry point scene.
	hovar::SplashScene* splash_scene_;
};

#endif // _RENDER_TARGET_APP_H