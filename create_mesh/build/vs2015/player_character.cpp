#include "player_character.h"
#include <iostream>

PlayerCharacter::PlayerCharacter()
{
	movement_speed = 1.0f;
}

PlayerCharacter::~PlayerCharacter()
{
	
}

void PlayerCharacter::Input(const gef::SonyController* controller_)
{
	//controller_->UpdateButtonStates(controller_->buttons_down());
	if (controller_->buttons_pressed() & gef_SONY_CTRL_CROSS)
	{
		std::cout << "x pressed" << std::endl;
	}
	if (controller_->buttons_down() & gef_SONY_CTRL_SQUARE)
	{
		std::cout << "square down" << std::endl;
	}
	if (controller_->buttons_released() & gef_SONY_CTRL_TRIANGLE)
	{
		std::cout << "tri released" << std::endl;
	}
	SetVelocity(gef::Vector4(controller_->left_stick_x_axis() * movement_speed, controller_->left_stick_y_axis() * -1 * movement_speed, 0.0f));
	UpdateVelocity();
}
