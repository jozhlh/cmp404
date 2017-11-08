#include "test_scene.h"

#include <sony_sample_framework.h>
#include <sony_tracking.h>

#include <system\platform.h>
#include <graphics\sprite_renderer.h>
#include <input\sony_controller_input_manager.h>

#include <iostream>

namespace hovar
{
	TestScene::TestScene()
	{
	}

	TestScene::~TestScene()
	{
	}

	void TestScene::Init(gef::Platform * platform)
	{
		platform_ = platform;
		input_manager_ = platform_->CreateInputManager();
		sprite_renderer_ = platform_->CreateSpriteRenderer();
		InitFont();
	}

	bool TestScene::Update(const float frame_time)
	{
		input_manager_->Update();

		if (input_manager_->controller_input()->GetController(0)->buttons_down() & gef_SONY_CTRL_CROSS)
		{
			// Load Next Scene
			std::cout << "x pressed" << std::endl;
		}
		return true;
	}

	void TestScene::Render()
	{
		RenderOverlay();
	}

	void TestScene::CleanUp()
	{
		delete sprite_renderer_;
		sprite_renderer_ = NULL;

		delete font_;
		font_ = NULL;
	}

	void TestScene::InitFont()
	{
		font_ = new gef::Font(*platform_);
		font_->Load("comic_sans");
	}

	void TestScene::DrawHUD()
	{
		if (font_)
		{
			font_->RenderText(sprite_renderer_,
				gef::Vector4((float)DISPLAY_WIDTH * 0.5f, (float)DISPLAY_HEIGHT * 0.2f, -0.9f), 3.0f,
				0xffffffff, gef::TJ_CENTRE, "hovAR", fps_);
			font_->RenderText(sprite_renderer_,
				gef::Vector4((float)DISPLAY_WIDTH * 0.5f, (float)DISPLAY_HEIGHT * 0.5f, -0.9f), 1.0f,
				0xffffffff, gef::TJ_CENTRE, "test scene", fps_);
		}
	}

	void TestScene::RenderOverlay()
	{
		//
		// render 2d hud on top
		//
		gef::Matrix44 proj_matrix2d;

		proj_matrix2d = platform_->OrthographicFrustum(0.0f, platform_->width(), 0.0f, platform_->height(), -1.0f, 1.0f);
		sprite_renderer_->set_projection_matrix(proj_matrix2d);
		sprite_renderer_->Begin(true);
		DrawHUD();
		sprite_renderer_->End();
	}
}
