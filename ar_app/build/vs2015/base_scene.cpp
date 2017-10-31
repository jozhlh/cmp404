#include "base_scene.h"
#include "system\platform.h"

namespace hovar
{
	BaseScene::BaseScene()
	{
		fps_ = 0.0f;
	}


	BaseScene::~BaseScene()
	{
		CleanUp();
	}

	void BaseScene::Init(gef::Platform* platform)
	{
		platform_ = platform;
		input_manager_ = platform_->CreateInputManager;
	}

	void BaseScene::CleanUp()
	{
		delete input_manager_;
		input_manager_ = NULL;
	}

	bool BaseScene::Update(const float frame_time)
	{
		fps_ = 1.0f / frame_time;
		input_manager_->Update();

		return true;
	}

	void BaseScene::Render()
	{
	}
}
