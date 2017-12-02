/*
	splash_scene.cpp

	First scene to be loaded.

	@author	Josh Hale
	Last Edited: 02/12/17
*/

#include "splash_scene.h"

#include <sony_sample_framework.h>
#include <sony_tracking.h>

#include <system\platform.h>
#include <graphics\sprite_renderer.h>
#include <input\sony_controller_input_manager.h>

#include <iostream>

namespace hovar
{
	SplashScene::SplashScene()
	{
	}

	SplashScene::~SplashScene()
	{
	}

	void SplashScene::Init(gef::Platform * platform)
	{
		platform_ = platform;
		input_manager_ = platform_->CreateInputManager();
		sprite_renderer_ = platform_->CreateSpriteRenderer();
		InitFont();
	}

	bool SplashScene::Update(const float frame_time)
	{
		input_manager_->Update();

		if (input_manager_->controller_input()->GetController(0)->buttons_down() & gef_SONY_CTRL_CROSS)
		{
			// Load Next Scene
			return false;
		}
		return true;
	}

	void SplashScene::Render()
	{
		RenderOverlay();
	}

	void SplashScene::CleanUp()
	{
		delete sprite_renderer_;
		sprite_renderer_ = NULL;

		delete font_;
		font_ = NULL;
	}

	void SplashScene::InitFont()
	{
		font_ = new gef::Font(*platform_);
		font_->Load("comic_sans");
	}

	void SplashScene::DrawHUD()
	{
		if (font_)
		{
			font_->RenderText(sprite_renderer_,
				gef::Vector4((float)DISPLAY_WIDTH * 0.5f, (float)DISPLAY_HEIGHT * 0.2f, -0.9f), 3.0f,
				0xffffffff, gef::TJ_CENTRE, "HOV/AR", fps_);
			font_->RenderText(sprite_renderer_,
				gef::Vector4((float)DISPLAY_WIDTH * 0.5f, (float)DISPLAY_HEIGHT * 0.5f, -0.9f), 1.0f,
				0xffffffff, gef::TJ_CENTRE, "Press X to play!", fps_);
		}
	}

	void SplashScene::RenderOverlay()
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
