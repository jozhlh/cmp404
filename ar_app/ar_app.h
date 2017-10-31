#ifndef _RENDER_TARGET_APP_H
#define _RENDER_TARGET_APP_H

#include <system/application.h>
#include <graphics/sprite.h>
#include <maths/vector2.h>
#include <vector>
#include <graphics/mesh_instance.h>
#include <platform/vita/graphics/texture_vita.h>
#include "build\vs2015\game_object.h"
#include "build\vs2015\game_object_manager.h"
#include "build\vs2015\player_character.h"
#include "build\vs2015\road_segment.h"
#include <graphics\model.h>
#include "assets\obj_loader.h"
#include <iostream>
#include "build\vs2015\pickup_manager.h"
#include "build\vs2015\cube_mesh.h"

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
	class SpriteRenderer;
	class Font;
	class Renderer3D;
	class Mesh;
	class OBJLoader;
	class RenderTarget;
	class TextureVita;
	class InputManager;
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
	void InitFont();
	void CleanUpFont();
	void DrawHUD();
	void RenderCameraFeed(struct AppData* dat);
	void Render3DScene();
	void RenderOverlay();
	void SetupLights();
	
	gef::InputManager* input_manager_;
	GameObjectManager* game_object_manager_;
	PickupManager* pickup_manager_;
	gef::SpriteRenderer* sprite_renderer_;
	gef::Font* font_;
	gef::Sprite* camera_display_;
	gef::TextureVita* camera_texture_;
	gef::Matrix44 orthographic_projection_;
	float kYScalingFactor_;
	PlayerCharacter* player_character_;
	RoadSegment* road_cross_;
	CubeMesh cube_builder_;
	gef::OBJLoader obj_loader_;
	gef::Matrix44 perspective_projection_fov_;
	
	float kGridSize;
	
	float fps_;

	class gef::Renderer3D* renderer_3d_;
};

#endif // _RENDER_TARGET_APP_H