#include "main_level.h"

#include <sony_sample_framework.h>

#include <system\platform.h>

namespace hovar
{

	MainLevel::MainLevel()
	{
		BaseScene::BaseScene();
	}

	MainLevel::~MainLevel()
	{
	}

	void MainLevel::Init(gef::Platform* platform)
	{
		BaseScene::Init(platform);
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

	void MainLevel::InitFont()
	{
		font_ = new gef::Font(platform_);
		font_->Load("comic_sans");
	}
}
