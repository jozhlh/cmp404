#include "main_level.h"

#include <sony_sample_framework.h>
#include <sony_tracking.h>

#include <system\platform.h>
#include <graphics\sprite_renderer.h>

namespace hovar
{

	MainLevel::MainLevel()
	{
		fps_ = 0.0f;
		score_ = 0.0f;
	}

	MainLevel::~MainLevel()
	{
	}

	void MainLevel::Init(gef::Platform* platform)
	{
		game_state_ = running;
		platform_ = platform;
		input_manager_ = platform_->CreateInputManager();
		sprite_renderer_ = platform_->CreateSpriteRenderer();
		renderer_3d_ = platform_->CreateRenderer3D();

		InitFont();
		InitLights();

		// Initialise vita rear camera display variables
		orthographic_projection_.SetIdentity();
		orthographic_projection_.OrthographicFrustumD3D(-1.0f, 1.0f, -1.0f, 
														1.0f, -1.0f, 1.0f);
		camera_display_ = new gef::Sprite();
		camera_display_->set_position(gef::Vector4(
			(float)DISPLAY_WIDTH / 2.0f, (float)DISPLAY_HEIGHT / 2.0f, 1.0f));
		camera_display_->set_width(((float)DISPLAY_WIDTH));
		camera_display_->set_height(((float)DISPLAY_HEIGHT*VIEW_SCALE_V));
		camera_texture_ = new gef::TextureVita();

		// initialise sony framework
		sampleInitialize();
		smartInitialize();

		// reset marker tracking
		AppData* dat = sampleUpdateBegin();
		smartTrackingReset();
		sampleUpdateEnd(dat);

		// create tracked object manager
		game_object_manager_ = new GameObjectManager();

		// create mesh transform
		float mv_scale = 0.00238f * 0.45f;
		gef::Transform mv_transform;
		mv_transform.set_scale(gef::Vector4(mv_scale, mv_scale, mv_scale));
		mv_transform.set_rotation_eulers(gef::Vector4(90.0f, 0.0f, 0.0f));
		mv_transform.set_translation(gef::Vector4(0.0f, 0.0f, 0.0f));

		gef::Matrix44 mesh_transform;
		road_model_ = new gef::Model();
		parent_model_ = new gef::Model();
		road_mesh_ = obj_loader_.LoadOBJToMesh("roadDefault.obj", *platform_, road_model_);
		parent_mesh_ = obj_loader_.LoadOBJToMesh("roadParent.obj", *platform_, parent_model_);
		for (int r = 0; r < NUM_OF_MARKERS; r++)
		{
			mesh_transform.SetIdentity();
			mesh_transform.SetTranslation(gef::Vector4(0.0f, 0.0f, mv_scale * 64.0f));
			road_[r] = new RoadSegment();
			road_[r]->SetMeshes(road_mesh_, parent_mesh_);
			gef::Vector4 collider_size = gef::Vector4(mv_scale * 88.f, mv_scale * 88.f, mv_scale * 88.f);
			road_[r]->SetCollider(cube_builder_.CreateCubeMesh(collider_size.x(), collider_size.y(), collider_size.z(), *platform_), mesh_transform, collider_size, "road");
			road_[r]->SetMvTransform(mv_transform);
			collider_size.set_x(mv_scale * 42.f);
			collider_size.set_y(mv_scale * 42.f);
			collider_size.set_z(mv_scale * 42.f);
			mesh_transform.SetIdentity();
			road_[r]->SetLocalTransformFromMatrix(mesh_transform);
			game_object_manager_->AddMarkerSpecificObject(road_[r]);
			road_[r]->SetParentMarkerID(r);
			road_[r]->SetAsParent(false);
		}
		road_[0]->SetAsParent(true);
		
		// create player character
		float player_scale = 0.5f * mv_scale;
		gef::Transform ship_transform;
		ship_transform.set_scale(gef::Vector4(player_scale, player_scale, player_scale));
		ship_transform.set_rotation_eulers(gef::Vector4(90.0f, 0.0f, 0.0f));
		ship_transform.set_translation(gef::Vector4(0.0f, 0.0f, 0.0f));

		player_character_ = new PlayerCharacter();
		player_character_->set_mesh(obj_loader_.LoadOBJToMesh("hovership.obj", *platform_, player_character_->GetModel()));
		mesh_transform.SetIdentity();
		mesh_transform.SetTranslation(gef::Vector4(0.0f, 0.0f, 0.005f));
		gef::Vector4 collider_size = gef::Vector4(player_scale * 24.0f, player_scale * 12.0f, player_scale * 25.0f);
		player_character_->SetCollider(cube_builder_.CreateCubeMesh(collider_size.x(), collider_size.y(), collider_size.z(), *platform_), mesh_transform, collider_size, "player");
		player_character_->SetMvTransform(ship_transform);
		mesh_transform.SetIdentity();
		mesh_transform.SetTranslation(gef::Vector4(0.0f, 0.0f, mv_scale * 80.0f));
		player_character_->SetLocalTransformFromMatrix(mesh_transform);
		player_character_->SetRespawnPosition(mesh_transform);
		game_object_manager_->GivePlayerReference(player_character_);
		player_character_->SetParentMarkerID(0);

		// create pickup manager and pickups
		float pickup_scale = 0.7f * mv_scale;
		pickup_manager_ = new PickupManager();
		pickup_manager_->Init(game_object_manager_,
			obj_loader_.LoadOBJToMesh("battery.obj", *platform_, pickup_manager_->GetModel()),
			cube_builder_.CreateCubeMesh(pickup_scale * 16.0f, pickup_scale * 16.0f,
										 pickup_scale * 16.0f, *platform_),
			mv_scale, pickup_scale / mv_scale);
	}

	bool MainLevel::Update(const float frame_time)
	{
		fps_ = 1.0f / frame_time;
		input_manager_->Update();
		if (game_state_ == running)
		{
			score_ += frame_time;
			::AppData* dat = sampleUpdateBegin();

			// use the tracking library to try and find markers
			smartUpdate(dat->currentImage);

			for (int marker_id = 0; marker_id < NUM_OF_MARKERS; marker_id++)
			{
				// check to see if a marker is being tracked
				if (sampleIsMarkerFound(marker_id))
				{
					game_object_manager_->SetMarkerVisiblity(marker_id, true);
					// marker is being tracked, get it’s transform
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

			for (int r = 0; r < NUM_OF_MARKERS; r++)
			{
				road_[r]->Update();
			}

			if (!game_object_manager_->PlayerRoadCollision(frame_time))
			{
				if (player_character_->IsAlive())
				{
					player_character_->Kill();
					pickup_manager_->Reset();
				}
			}

			if (pickup_manager_->PlayerPickupCollision(player_character_))
			{
				player_character_->Stop();
				game_object_manager_->FindNewParent();
				score_ += 10.0f;
			}
			pickup_manager_->Update(frame_time);

			if (player_character_->GetEnergy() < 0.0f)
			{
				game_state_ = finished;
			}

			sampleUpdateEnd(dat);
		}
		else
		{
			// wait for user to press x or o
			if (input_manager_->controller_input()->GetController(0)->buttons_pressed() & gef_SONY_CTRL_CROSS)
			{
				Restart();
			}
			else if (input_manager_->controller_input()->GetController(0)->buttons_pressed() & gef_SONY_CTRL_CIRCLE)
			{
				return false;
			}
		}

		return true;
	}

	void MainLevel::Render()
	{
		AppData* dat = sampleRenderBegin();

		if (game_state_ == running)
		{
			// Render the camera feed
			RenderCameraFeed(dat);

			// Render 3D scene
			Render3DScene();

			RenderOverlay();
		}
		else
		{
			RenderGameOver();
		}
		sampleRenderEnd();
	}

	void MainLevel::DrawHUD()
	{
		if (font_)
		{
			// fps counter
			font_->RenderText(sprite_renderer_, 
							  gef::Vector4(DISPLAY_WIDTH - 10.0f, 10.0f, -0.9f), 1.0f,
							  0xffffffff, gef::TJ_RIGHT, "FPS: %.1f", fps_);

			// energy level
			font_->RenderText(sprite_renderer_,
				gef::Vector4(10.0f, 10.0f, -0.9f), 1.0f,
				0xffffffff, gef::TJ_LEFT, "ENERGY: %.0f", player_character_->GetEnergy());

			// score
			font_->RenderText(sprite_renderer_,
				gef::Vector4(DISPLAY_WIDTH * 0.5f, 10.0f, -0.9f), 1.0f,
				0xffffffff, gef::TJ_CENTRE, "SCORE: %.0f", score_);
		}
	}

	void MainLevel::RenderCameraFeed(struct ::AppData* dat)
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

	void MainLevel::Render3DScene()
	{
		// SET VIEW AND PROJECTION MATRIX HERE
		float near_clip = 0.05f;
		float far_clip = 10.0f;

		gef::Matrix44 scale_matrix;
		scale_matrix.Scale(gef::Vector4(1.0f, VIEW_SCALE_V, 1.0f, 0.0f));

		perspective_projection_fov_ = platform_->PerspectiveProjectionFov(SCE_SMART_IMAGE_FOV, (float)SCE_SMART_IMAGE_WIDTH / (float)SCE_SMART_IMAGE_HEIGHT, near_clip, far_clip);

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

	void MainLevel::RenderOverlay()
	{
		//
		// render 2d hud on top
		//
		gef::Matrix44 proj_matrix2d;

		proj_matrix2d = platform_->OrthographicFrustum(0.0f, platform_->width(), 0.0f, platform_->height(), -1.0f, 1.0f);
		sprite_renderer_->set_projection_matrix(proj_matrix2d);
		sprite_renderer_->Begin(false);
		DrawHUD();
		sprite_renderer_->End();
	}

	void MainLevel::RenderGameOver()
	{
		renderer_3d_->Begin();
		renderer_3d_->End();
		//
		// render 2d hud on top
		//
		gef::Matrix44 proj_matrix2d;

		proj_matrix2d = platform_->OrthographicFrustum(0.0f, platform_->width(), 0.0f, platform_->height(), -1.0f, 1.0f);
		sprite_renderer_->set_projection_matrix(proj_matrix2d);
		sprite_renderer_->Begin(false);
		if (font_)
		{
			// fps counter
			font_->RenderText(sprite_renderer_,
				gef::Vector4(DISPLAY_WIDTH * 0.5f, DISPLAY_HEIGHT * 0.4f, -0.9f), 1.0f,
				0xffffffff, gef::TJ_CENTRE, "GAME OVER");

			// energy level
			font_->RenderText(sprite_renderer_,
				gef::Vector4(DISPLAY_WIDTH * 0.5f, DISPLAY_HEIGHT * 0.5f, -0.9f), 1.0f,
				0xffffffff, gef::TJ_CENTRE, "SCORE: %.0f", score_);

			// score
			font_->RenderText(sprite_renderer_,
				gef::Vector4(DISPLAY_WIDTH * 0.5f, DISPLAY_HEIGHT * 0.6f, -0.9f), 1.0f,
				0xffffffff, gef::TJ_CENTRE, "'X' To Restart        'O' To Quit");
		}
		sprite_renderer_->End();
	}

	void MainLevel::Restart()
	{
		score_ = 0.0f;
		player_character_->ResetEnergy();
		player_character_->Respawn();
		player_character_->Stop();
		pickup_manager_->Reset();
		game_state_ = running;
	}

	void MainLevel::InitFont()
	{
		font_ = new gef::Font(*platform_);
		font_->Load("comic_sans");
	}

	void MainLevel::InitLights()
	{
		gef::PointLight point_light;
		point_light.set_colour(gef::Colour(0.7f, 0.7f, 1.0f, 1.0f));
		point_light.set_position(gef::Vector4(-500.0f, 400.0f, 700.0f));

		gef::Default3DShaderData& shader_data = renderer_3d_->default_shader_data();
		shader_data.set_ambient_light_colour(gef::Colour(0.5f, 0.5f, 0.5f, 1.0f));
		shader_data.AddPointLight(point_light);
	}

	void MainLevel::CleanUp()
	{
		if (input_manager_)
		{
			delete input_manager_;
			input_manager_ = NULL;
		}

		if (renderer_3d_)
		{
			delete renderer_3d_;
			renderer_3d_ = NULL;
		}

		if (sprite_renderer_)
		{
			delete sprite_renderer_;
			sprite_renderer_ = NULL;
		}

		if (font_)
		{
			delete font_;
			font_ = NULL;
		}

		if (camera_display_)
		{
			delete camera_display_;
			camera_display_ = NULL;
		}

		if (camera_texture_)
		{
			//delete camera_texture_;
			//camera_texture_ = NULL;
		}

		if (game_object_manager_)
		{
			delete game_object_manager_;
			game_object_manager_ = NULL;
		}

		if (pickup_manager_)
		{
			delete pickup_manager_;
			pickup_manager_ = NULL;
		}

		if (player_character_)
		{
			delete player_character_;
			player_character_ = NULL;
		}
		
		for (int r = 0; r < NUM_OF_MARKERS; r++)
		{
			if (road_[r])
			{
				delete road_[r];
				road_[r] = NULL;
			}
		}

		if (road_mesh_)
		{
			delete road_mesh_;
			road_mesh_ = NULL;
		}

		if (parent_mesh_)
		{
			delete parent_mesh_;
			parent_mesh_ = NULL;
		}

		if (road_model_)
		{
			delete road_model_;
			road_model_ = NULL;
		}

		if (parent_model_)
		{
			delete parent_model_;
			parent_model_ = NULL;
		}

		smartRelease();
		sampleRelease();
	}
}
