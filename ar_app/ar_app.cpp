/*
	ar_app.cpp

	Application class which runs the hovar game.

	@author	Josh Hale
	Last Edited: 02/12/17
*/

#include "ar_app.h"

#include <iostream>

ARApp::ARApp(gef::Platform& platform) :
	Application(platform)
{
}

void ARApp::Init()
{
	splash_scene_ = new hovar::SplashScene();
	current_scene_ = splash_scene_;
	current_scene_->Init(&platform_);
}

void ARApp::CleanUp()
{
	if (current_scene_ != NULL)
	{
		current_scene_->CleanUp();
		delete current_scene_;
		current_scene_ = NULL;
	}
}

bool ARApp::Update(float frame_time)
{
	if (!current_scene_->Update(frame_time))
	{
		std::cout << "Update false" << std::endl;
		SwitchScene();
	}
	return true;
}

void ARApp::Render()
{
	current_scene_->Render();
}

void ARApp::SwitchScene()
{
	if (current_scene_ == splash_scene_)
	{
		CleanUp();
		main_scene_ = new hovar::MainLevel();
		current_scene_ = main_scene_;
	}
	else
	{
		CleanUp();
		splash_scene_ = new hovar::SplashScene();
		current_scene_ = splash_scene_;
	}
	current_scene_->Init(&platform_);
}
