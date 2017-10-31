#include "ar_app.h"
#include <system/platform.h>
#include <graphics/sprite_renderer.h>
#include <graphics/texture.h>
#include <graphics/mesh.h>
#include <graphics/primitive.h>
#include <assets/png_loader.h>
#include <graphics/image_data.h>
#include <graphics/font.h>
#include <input/touch_input_manager.h>
#include <maths/vector2.h>
#include <input/input_manager.h>
#include <maths/math_utils.h>
#include <graphics/renderer_3d.h>
#include <graphics/render_target.h>
#include <maths\transform.h>
#include "build\vs2015\road_configs.h"
#include <sony_sample_framework.h>
#include <sony_tracking.h>


ARApp::ARApp(gef::Platform& platform) :
	Application(platform),
	input_manager_(NULL),
	sprite_renderer_(NULL),
	font_(NULL),
	renderer_3d_(NULL)
{
}

void ARApp::Init()
{
  kGridSize = 0.05f;
  input_manager_ = platform_.CreateInputManager();
  sprite_renderer_ = platform_.CreateSpriteRenderer();
  renderer_3d_ = platform_.CreateRenderer3D();
  
  InitFont();

	// Initialise camera variables
	orthographic_projection_.SetIdentity();
	orthographic_projection_.OrthographicFrustumD3D(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
	kYScalingFactor_ = (((float)DISPLAY_WIDTH / (float)DISPLAY_HEIGHT) / ((float)CAMERA_WIDTH / (float)CAMERA_HEIGHT));
	camera_display_ = new gef::Sprite();
	camera_display_->set_position(gef::Vector4((float)DISPLAY_WIDTH / 2.0f, (float)DISPLAY_HEIGHT / 2.0f, 1.0f));
	camera_display_->set_width(((float)DISPLAY_WIDTH));
	camera_display_->set_height(((float)DISPLAY_HEIGHT * kYScalingFactor_));
	camera_texture_ = new gef::TextureVita();

	// initialise sony framework
	sampleInitialize();
	smartInitialize();

	// reset marker tracking
	AppData* dat = sampleUpdateBegin();
	smartTrackingReset();
	sampleUpdateEnd(dat);

	SetupLights();

	// create tracked object manager
	game_object_manager_ = new GameObjectManager();

	// create mesh transform
	float mv_scale = 0.00238f;
	gef::Transform mv_transform;
	mv_transform.set_scale(gef::Vector4(mv_scale, mv_scale, mv_scale));
	mv_transform.set_rotation_eulers(gef::Vector4(90.0f, 0.0f, 0.0f));
	mv_transform.set_translation(gef::Vector4(0.0f, 0.0f, 0.0f));

	gef::Matrix44 mesh_transform;
	mesh_transform.SetIdentity();
	road_cross_ = new RoadSegment();
	road_cross_->set_mesh(obj_loader_.LoadOBJToMesh("cross.obj", platform_, road_cross_->Model()));
	road_cross_->SetCollider(cube_builder_.CreateCubeMesh(mv_scale * 126.f, mv_scale * 42.f, mv_scale * 126.f, platform_), mesh_transform);
	road_cross_->SetMvTransform(mv_transform);
	road_cross_->SetLocalTransformFromMatrix(mesh_transform);
	road_cross_->InitWallCollisionBoxes(cross, cube_builder_.CreateCubeMesh(mv_scale * 42.f, mv_scale * 42.f, mv_scale * 42.f, platform_), mv_scale * 42.f, mesh_transform);
	game_object_manager_->AddMarkerSpecificObject(road_cross_);
	road_cross_->SetParentMarker(0);
	

	// create player character
	float player_scale = 0.3f;
	gef::Transform ship_transform;
	ship_transform.set_scale(gef::Vector4(mv_scale * player_scale, mv_scale * player_scale, mv_scale * player_scale));
	ship_transform.set_rotation_eulers(gef::Vector4(90.0f, 0.0f, 0.0f));
	ship_transform.set_translation(gef::Vector4(0.0f, 0.0f, 0.0f));

	player_character_ = new PlayerCharacter();
	player_character_->set_mesh(obj_loader_.LoadOBJToMesh("hovership.obj", platform_, player_character_->Model()));
	mesh_transform.SetIdentity();
	mesh_transform.SetTranslation(gef::Vector4(0.0f, 0.0f, 0.01f));
	player_character_->SetCollider(cube_builder_.CreateCubeMesh(mv_scale * player_scale * 24.0f, mv_scale * player_scale * 12.0f, mv_scale * player_scale * 25.0f, platform_), mesh_transform);
	player_character_->SetMvTransform(ship_transform);
	mesh_transform.SetIdentity();
	mesh_transform.SetTranslation(gef::Vector4(0.0f, 0.0f, 0.02f));
	player_character_->SetLocalTransformFromMatrix(mesh_transform);
	player_character_->SetRespawnPosition(mesh_transform);
	game_object_manager_->GivePlayerReference(player_character_);
	player_character_->SetParentMarker(0);

	// create pickup manager and pickups
	float pickup_scale = 0.2f;
	pickup_manager_ = new PickupManager();
	pickup_manager_->Init(game_object_manager_, 
		obj_loader_.LoadOBJToMesh("ref42.obj", platform_, pickup_manager_->Model()),
		cube_builder_.CreateCubeMesh(mv_scale * pickup_scale * 42.0f, mv_scale * pickup_scale * 42.0f, mv_scale * pickup_scale * 42.0f, platform_),
		mv_scale, pickup_scale);
}

void ARApp::CleanUp()
{
	smartRelease();
	sampleRelease();

	CleanUpFont();
	delete sprite_renderer_;
	sprite_renderer_ = NULL;

	delete renderer_3d_;
	renderer_3d_ = NULL;

	delete input_manager_;
	input_manager_ = NULL;

	delete camera_display_;
	camera_display_ = NULL;

	delete camera_texture_;
	camera_texture_ = NULL;

	delete game_object_manager_;
	game_object_manager_ = NULL;
}

bool ARApp::Update(float frame_time)
{
	fps_ = 1.0f / frame_time;

	input_manager_->Update();

	AppData* dat = sampleUpdateBegin();

	// use the tracking library to try and find markers
	smartUpdate(dat->currentImage);

	for (int marker_id = 0; marker_id < NUM_OF_MARKERS; marker_id++)
	{
		// check to see if a marker is being tracked
		if (sampleIsMarkerFound(marker_id))
		{
			game_object_manager_->SetMarkerVisiblity(marker_id, true);
			// marker is being tracked, get it�s transform
			gef::Matrix44 marker_transform;
			sampleGetTransform(
				marker_id,
				&marker_transform);

			game_object_manager_->SetMarkerPosition(marker_id, marker_transform);
		}
		else
		{
			game_object_manager_->SetMarkerVisiblity(marker_id, false);
		}
	}
	
	game_object_manager_->UpdateMarkerData();
	
	player_character_->Update(input_manager_->controller_input()->GetController(0), frame_time);
	road_cross_->Update();

	if (!game_object_manager_->PlayerRoadCollision())
	{
		player_character_->Respawn();
	}

	gef::Vector4 collision_vector;
	if (game_object_manager_->PlayerWallCollision(&collision_vector))
	{
		player_character_->Rebound(collision_vector);
	}
	
	pickup_manager_->PlayerPickupCollision(player_character_);
	pickup_manager_->Update(frame_time);


	sampleUpdateEnd(dat);

	return true;
}

void ARApp::Render()
{
	AppData* dat = sampleRenderBegin();

	// Render the camera feed
	RenderCameraFeed(dat);
	
	// Render 3D scene
	Render3DScene();
	
	RenderOverlay();

	sampleRenderEnd();
}

void ARApp::RenderCameraFeed(AppData * dat)
{
	// REMEMBER AND SET THE PROJECTION MATRIX HERE
	renderer_3d_->set_projection_matrix(orthographic_projection_);
	sprite_renderer_->Begin(true);

	// DRAW CAMERA FEED SPRITE HERE
	// check there is data present for the camera image before trying to draw it
	if (dat->currentImage)
	{
		camera_texture_->set_texture(dat->currentImage->tex_yuv);
		camera_display_->set_texture(camera_texture_);
		sprite_renderer_->DrawSprite(*camera_display_);
	}

	sprite_renderer_->End();
}

void ARApp::Render3DScene()
{
	// SET VIEW AND PROJECTION MATRIX HERE
	float near_clip = 0.05f;
	float far_clip = 10.0f;

	gef::Matrix44 scale_matrix;
	scale_matrix.Scale(gef::Vector4(1.0f, kYScalingFactor_, 1.0f, 0.0f));

	perspective_projection_fov_ = platform_.PerspectiveProjectionFov(SCE_SMART_IMAGE_FOV, (float)SCE_SMART_IMAGE_WIDTH / (float)SCE_SMART_IMAGE_HEIGHT, near_clip, far_clip);

	perspective_projection_fov_ = perspective_projection_fov_ * scale_matrix;
	renderer_3d_->set_projection_matrix(perspective_projection_fov_);

	gef::Matrix44 viewMatrix;
	viewMatrix.SetIdentity();
	renderer_3d_->set_view_matrix(viewMatrix);


	// Begin rendering 3D meshes, don't clear the frame buffer
	renderer_3d_->Begin(false);

	// DRAW 3D MESHES HERE
	// check to see if a marker is being tracked
	game_object_manager_->RenderObjects(renderer_3d_);

	renderer_3d_->End();
}

void ARApp::RenderOverlay()
{
	//
	// render 2d hud on top
	//
	gef::Matrix44 proj_matrix2d;

	proj_matrix2d = platform_.OrthographicFrustum(0.0f, platform_.width(), 0.0f, platform_.height(), -1.0f, 1.0f);
	sprite_renderer_->set_projection_matrix(proj_matrix2d);
	sprite_renderer_->Begin(false);
	DrawHUD();
	sprite_renderer_->End();
}

void ARApp::DrawHUD()
{
	if (font_)
	{
		font_->RenderText(sprite_renderer_, gef::Vector4(850.0f, 510.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "FPS: %.1f", fps_);
	}
}

void ARApp::InitFont()
{
	font_ = new gef::Font(platform_);
	font_->Load("comic_sans");
}

void ARApp::CleanUpFont()
{
	delete font_;
	font_ = NULL;
}

void ARApp::SetupLights()
{
	gef::PointLight default_point_light;
	default_point_light.set_colour(gef::Colour(0.7f, 0.7f, 1.0f, 1.0f));
	default_point_light.set_position(gef::Vector4(-500.0f, 400.0f, 700.0f));

	gef::Default3DShaderData& default_shader_data = renderer_3d_->default_shader_data();
	default_shader_data.set_ambient_light_colour(gef::Colour(0.5f, 0.5f, 0.5f, 1.0f));
	default_shader_data.AddPointLight(default_point_light);
}