#include "mesh_app.h"
#include <system/platform.h>
#include <graphics/sprite_renderer.h>
#include <graphics/texture.h>
#include <graphics/mesh.h>
#include <graphics/primitive.h>
#include <assets/png_loader.h>
#include <assets/obj_loader.h>
#include <graphics/image_data.h>
#include <graphics/font.h>
#include <input/touch_input_manager.h>
#include <maths/vector2.h>
#include <input/sony_controller_input_manager.h>
#include <maths/math_utils.h>
#include <graphics/renderer_3d.h>

MeshApp::MeshApp(gef::Platform& platform) :
	Application(platform),
	sprite_renderer_(NULL),
	font_(NULL),
	renderer_3d_(NULL)
{
}

void MeshApp::Init()
{
	sprite_renderer_ = gef::SpriteRenderer::Create(platform_);
	renderer_3d_ = gef::Renderer3D::Create(platform_);
	input_manager_ = gef::InputManager::Create(platform_);

	InitFont();

	mesh_ = CreateCubeMesh();
	cube_player_.set_mesh(mesh_);

	// Import model
	gef::OBJLoader obj_loader;
	obj_loader.Load("ball1.obj", platform_, model_ball1_);
	gef::Sphere new_bounds(gef::Vector4(0.0f, 0.0f, 0.0f), 50.5f);
	model_ball1_.mesh()->set_bounding_sphere(new_bounds);
	//if (!player_ball1_)
	//{
		player_ball1_ = new PlayerCharacter();
	//}
	player_ball1_->set_mesh(model_ball1_.mesh());
	player_ball1_->Translate(gef::Vector4(-1.f, 1.0f, 2.0f, 0.0f));
	// Set the transform matrix for the player
	//player_transform_ball1_.SetIdentity();
	//player_ball1_.set_transform(player_transform_ball1_);

	obj_loader.Load("ball2.obj", platform_, model_ball2_);
	model_ball2_.mesh()->set_bounding_sphere(new_bounds);
	player_ball2_.set_mesh(model_ball2_.mesh());
	// Set the transform matrix for the player
	player_transform_ball2_.SetIdentity();
	player_transform_ball2_.SetTranslation(gef::Vector4(-90.0f, 90.0f, 0.0f, 0.0f));
	player_ball2_.set_transform(player_transform_ball2_);

	aabb_collision = false;
	spherical_collision = false;

	camera_ = new Camera();
	SetupLights();
}

void MeshApp::CleanUp()
{
	delete mesh_;
	mesh_ = NULL;

	CleanUpFont();
	delete sprite_renderer_;
	sprite_renderer_ = NULL;

	delete renderer_3d_;
	renderer_3d_ = NULL;

	model_ball1_.Release();
	model_ball2_.Release();
}

bool MeshApp::Update(float frame_time)
{
	fps_ = 1.0f / frame_time;
	float rotSpeed = 10.0f * frame_time;
	aabb_collision = CollisionAABB(player_ball1_, &player_ball2_);
	spherical_collision = CollisionSpherical(player_ball1_, &player_ball2_);
	player_ball1_->Rotate(gef::Vector4(0.0f, rotSpeed, 0.0f, 0.0f));
	input_manager_->Update();
	player_ball1_->Input(input_manager_->controller_input()->GetController(0));
	return true;
}

void MeshApp::Render()
{
	renderer_3d_->set_projection_matrix(camera_->CalculateProjectionMatrix(platform_));
	renderer_3d_->set_view_matrix(camera_->CalculateViewMatrix());

	// draw meshes here
	renderer_3d_->Begin();

	renderer_3d_->DrawMesh(cube_player_);
	renderer_3d_->DrawMesh(*player_ball1_);
	renderer_3d_->DrawMesh(player_ball2_);
	renderer_3d_->End();

	// setup the sprite renderer, but don't clear the frame buffer
	// draw 2D sprites here
	sprite_renderer_->Begin(false);
	DrawHUD();
	ReportCollision();
	sprite_renderer_->End();
}

void MeshApp::InitFont()
{
	font_ = new gef::Font(platform_);
	font_->Load("comic_sans");
}

void MeshApp::CleanUpFont()
{
	delete font_;
	font_ = NULL;
}

void MeshApp::DrawHUD()
{
	if(font_)
	{
		// display frame rate
		font_->RenderText(sprite_renderer_, gef::Vector4(850.0f, 510.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "FPS: %.1f", fps_);
		font_->RenderText(sprite_renderer_, gef::Vector4(750.0f, 370.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "boundingRad %.1f", player_ball1_->mesh()->bounding_sphere().radius());
		font_->RenderText(sprite_renderer_, gef::Vector4(750.0f, 410.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "ball1rotX %.1f", player_ball1_->GetTransform()->rotation_eulers().x());
		font_->RenderText(sprite_renderer_, gef::Vector4(750.0f, 440.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "ball1rotY %.1f", player_ball1_->GetTransform()->rotation_eulers().y());
		font_->RenderText(sprite_renderer_, gef::Vector4(750.0f, 470.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "ball1rotZ %.1f", player_ball1_->GetTransform()->rotation_eulers().z());
	}
}

void MeshApp::ReportCollision()
{
	if (font_)
	{
		if (spherical_collision)
		{
			font_->RenderText(sprite_renderer_, gef::Vector4(100.0f, 50.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "spherical collision");
		}
		if (aabb_collision)
		{
			font_->RenderText(sprite_renderer_, gef::Vector4(100.0f, 100.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "aabb collision");
		}
	}
}

gef::Mesh* MeshApp::CreateCubeMesh()
{
	gef::Mesh* mesh = platform_.CreateMesh();

	// initialise the vertex data to create a 1, 1, 1 cube
	const float half_size = 0.5f;
	const gef::Mesh::Vertex vertices[] = {
		{ half_size, -half_size, -half_size,  0.577f, -0.577f, -0.577f, 0.0f, 0.0f},
		{ half_size,  half_size, -half_size,  0.577f,  0.577f, -0.577f, 0.0f, 0.0f},
		{-half_size,  half_size, -half_size, -0.577f,  0.577f, -0.577f, 0.0f, 0.0f},
		{-half_size, -half_size, -half_size, -0.577f, -0.577f, -0.577f, 0.0f, 0.0f},
		{ half_size, -half_size,  half_size,  0.577f, -0.577f,  0.577f, 0.0f, 0.0f},
		{ half_size,  half_size,  half_size,  0.577f,  0.577f,  0.577f, 0.0f, 0.0f},
		{-half_size,  half_size,  half_size, -0.577f,  0.577f,  0.577f, 0.0f, 0.0f},
		{-half_size, -half_size,  half_size, -0.577f, -0.577f,  0.577f, 0.0f, 0.0f}	
	};

	mesh->InitVertexBuffer(platform_, static_cast<const void*>(vertices), sizeof(vertices)/sizeof(gef::Mesh::Vertex), sizeof(gef::Mesh::Vertex));

	// we will create a single triangle list primitive to draw the triangles that make up the cube
	mesh->AllocatePrimitives(1);
	gef::Primitive* primitive = mesh->GetPrimitive(0);

	const UInt32 indices[] = {
    // Back
    0, 1, 2,
    2, 3, 0,
    // Front
    6, 5, 4,
    4, 7, 6,
    // Left
    2, 7, 3,
    2, 6, 7,
    // Right
    0, 4, 1,
	5, 1, 4,
    // Top
    6, 2, 1, 
	5, 6, 1,
	// Bottom
    0, 3, 7,
    0, 7, 4 
	};

	primitive->InitIndexBuffer(platform_, static_cast<const void*>(indices), sizeof(indices)/sizeof(UInt32),sizeof(UInt32));
	primitive->set_type(gef::TRIANGLE_LIST);

	// set size of bounds, we need this for collision detection routines
	gef::Aabb aabb(gef::Vector4(-half_size, -half_size, -half_size), gef::Vector4(half_size, half_size, half_size));
	gef::Sphere sphere(aabb);

	mesh->set_aabb(aabb);
	mesh->set_bounding_sphere(sphere);

	return mesh;
}

void MeshApp::SetupLights()
{
	gef::PointLight default_point_light;
	default_point_light.set_colour(gef::Colour(0.7f, 0.7f, 1.0f, 1.0f));
	default_point_light.set_position(gef::Vector4(-500.0f, 400.0f, 700.0f));

	gef::Default3DShaderData& default_shader_data = renderer_3d_->default_shader_data();
	default_shader_data.set_ambient_light_colour(gef::Colour(0.5f, 0.5f, 0.5f, 1.0f));
	default_shader_data.AddPointLight(default_point_light);
}

bool MeshApp::CollisionSpherical(gef::MeshInstance* collider_mesh_1_, gef::MeshInstance* collider_mesh_2_)
{
	// Calculate the squared distance between the centers of both spheres
	gef::Vector4 collider_1_pos = collider_mesh_1_->mesh()->bounding_sphere().position().Transform(collider_mesh_1_->transform());
	gef::Vector4 collider_2_pos = collider_mesh_2_->mesh()->bounding_sphere().position().Transform(collider_mesh_2_->transform());
	float distance_squared = gef::Vector4(collider_1_pos - collider_2_pos).LengthSqr();

	// Calculate the squared sum of both radii
	float radii_sum_squared = collider_mesh_1_->mesh()->bounding_sphere().radius() + collider_mesh_2_->mesh()->bounding_sphere().radius();
	radii_sum_squared *= radii_sum_squared;

	// if the the distance squared is less than or equal to the square sum of the radii, then there is a collision
	if (distance_squared <= radii_sum_squared)
	{
		return true;
	}

	// if the distance squared is greater than the sum of the radii squared there is no collision
	return false;
}

bool MeshApp::CollisionAABB(gef::MeshInstance * collider_mesh_1_, gef::MeshInstance * collider_mesh_2_)
{
	// Get the transformed min and max vertices
	gef::Vector4 min_1 = collider_mesh_1_->mesh()->aabb().min_vtx().Transform(collider_mesh_1_->transform());
	gef::Vector4 max_1 = collider_mesh_1_->mesh()->aabb().max_vtx().Transform(collider_mesh_1_->transform());
	gef::Vector4 min_2 = collider_mesh_2_->mesh()->aabb().min_vtx().Transform(collider_mesh_2_->transform());
	gef::Vector4 max_2 = collider_mesh_2_->mesh()->aabb().max_vtx().Transform(collider_mesh_2_->transform());

	return(max_1.x() > min_2.x()
		&& min_1.x() < max_2.x()
		&& max_1.y() > min_2.y()
		&& min_1.y() < max_2.y()
		&& max_1.z() > min_2.z()
		&& min_1.z() < max_2.z());
}


