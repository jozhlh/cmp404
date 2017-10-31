#ifndef _MESH_APP_H
#define _MESH_APP_H

#include <system/application.h>
#include <graphics/sprite.h>
#include <maths/vector2.h>
#include <maths/vector4.h>
#include <vector>
#include <graphics/mesh_instance.h>
#include <graphics/material.h>
#include <graphics/model.h>
#include <game_object.h>
#include <player_character.h>
#include <input/input_manager.h>
#include <camera_manager.h>

// FRAMEWORK FORWARD DECLARATIONS
namespace gef
{
	class Platform;
	class SpriteRenderer;
	class Font;
	class Renderer3D;
	class Mesh;
}

class MeshApp : public gef::Application
{
public:
	MeshApp(gef::Platform& platform);
	void Init();
	void CleanUp();
	bool Update(float frame_time);
	void Render();
private:
	void InitFont();
	void CleanUpFont();
	void DrawHUD();
	void ReportCollision();
	void SetupLights();
	//void SetupCamera();
	bool CollisionSpherical(gef::MeshInstance* collider_mesh_1_, gef::MeshInstance* collider_mesh_2_);
	bool CollisionAABB(gef::MeshInstance* collider_mesh_1_, gef::MeshInstance* collider_mesh_2_);

	gef::Mesh* CreateCubeMesh();

	gef::SpriteRenderer* sprite_renderer_;
	gef::Font* font_;
	gef::InputManager * input_manager_;
	Camera* camera_;

	float fps_;

	class gef::Renderer3D* renderer_3d_;
	class gef::Mesh* mesh_;
	gef::MeshInstance cube_player_;
	gef::Model model_ball1_;
	PlayerCharacter* player_ball1_;
	//gef::Matrix44 player_transform_ball1_;
	gef::Model model_ball2_;
	gef::MeshInstance player_ball2_;
	gef::Matrix44 player_transform_ball2_;


/*
	gef::Vector4 camera_eye;
	gef::Vector4 camera_lookat;
	gef::Vector4 camera_up;
	float camera_fov;
	float near_plane;
	float far_plane;
*/
	bool spherical_collision;
	bool aabb_collision;
};

#endif // _MESH_APP_H
