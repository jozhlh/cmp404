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

class ARApp : public gef::Application
{
public:
	ARApp(gef::Platform& platform);
	void Init();
	void CleanUp();
	bool Update(float frame_time);
	void Render();

private:
	void SwitchScene();

	hovar::BaseScene* current_scene_;
	hovar::MainLevel* main_scene_;
	hovar::SplashScene* splash_scene_;
};

#endif // _RENDER_TARGET_APP_H