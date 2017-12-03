/*
	player_character.cpp

	The player controlled vehicle.

	@author	Josh Hale
	Last Edited: 02/12/17
*/

#include "player_character.h"
#include <iostream>
#include "graphics\mesh.h"
#define STICK_DEAD_ZONE 0.05f

namespace hovar
{
	PlayerCharacter::PlayerCharacter()
	{
		max_speed_ = 0.01f;
		acceleration_ = 0.0001f;
		deceleration_ = -0.0001f;

		turning_speed_ = 3.0f;
		SetVelocity(gef::Vector4(0.0f, 0.0f, 0.0f));
		SetDrag(0.000025f);
		SetDeadZone(0.000002f);

		current_energy_ = 20.0f;
		energy_decay_rate_ = 1.0f;
		pickup_duration_ = 1.0f;
		dead_duration_ = 0.5f;
		pickup_counter_ = 0.0f;
		dead_counter_ = 0.0f;
		state_ = alive;
	}

	PlayerCharacter::~PlayerCharacter()
	{
		delete collider_;
		collider_ = NULL;
	}


	void PlayerCharacter::PickupAnim(float dt)
	{
		state_ = alive;
	}

	void PlayerCharacter::DeadAnim(float dt)
	{
		dead_counter_ += dt;
		gef::Vector4 input_velocity = GetLocalTransform()->GetMatrix().GetColumn(2);
		input_velocity.set_z(input_velocity.z() * -0.0004f);
		ApplyAcceleration(input_velocity);
		if (dead_counter_ > dead_duration_)
		{
			state_ = alive;
			Respawn();
		}
	}

	gef::Vector4 PlayerCharacter::Input(const gef::SonyController* controller, float dt)
	{
		float forward_acceleration = 0.0f;

		// Apply acceleration/deceleration to velocity
		if (controller->buttons_down() & gef_SONY_CTRL_CROSS)
		{
			forward_acceleration = acceleration_;
		}
		else if (controller->buttons_down() & gef_SONY_CTRL_SQUARE)
		{
			forward_acceleration = deceleration_;
		}

		// left stick sets rotation
		float amountToRotate = 0.0f;
		if ((controller->left_stick_x_axis() > STICK_DEAD_ZONE) || (controller->left_stick_x_axis() < STICK_DEAD_ZONE))
		{
			amountToRotate = controller->left_stick_x_axis() * turning_speed_;
			GetLocalTransform()->Rotate(gef::Vector4(0.0f, 0.0f, -1 * amountToRotate));
		}

		// apply acceleration to forward vetor
		gef::Vector4 input_velocity = GetLocalTransform()->GetMatrix().GetColumn(1);

		input_velocity.set_x(-1 * input_velocity.x() * forward_acceleration);
		input_velocity.set_y(input_velocity.y() * forward_acceleration);

		return input_velocity;
	}

	void PlayerCharacter::Update(const gef::SonyController* controller, float dt)
	{
		current_energy_ -= (energy_decay_rate_ * dt);
		if (state_ == alive)
		{
			// Get input
			ApplyAcceleration(Input(controller, dt));
		}
		else if (state_ == dead)
		{
			DeadAnim(dt);
		}
		else if (state_ == pickup)
		{
			PickupAnim(dt);
		}
		// Apply drag to velocity
		VelocityDrag();
		VelocityLimits(max_speed_);
		// Update position based on velocity
		UpdateVelocity();
	}

	void PlayerCharacter::UpdateMeshTransform()
	{
		transform_ = m_mv_transform_.GetMatrix() * m_transform_->GetMatrix();
		collider_->set_transform(collider_offset_ * m_transform_->GetMatrix());
		if (obb_ != NULL)
		{
			obb_->SetCoordinateFrameFromMatrix(collider_offset_ * m_transform_->GetMatrix(), "player");
		}
	}

	void PlayerCharacter::Render(gef::Renderer3D * renderer)
	{
		if (parent_visible_)
		{
			renderer->DrawMesh(*this);
			//renderer->DrawMesh(*collider_);
		}
	}

	void PlayerCharacter::Respawn()
	{
		SetVelocity(gef::Vector4(0.0f, 0.0f, 0.0f));
		SetLocalTransformFromMatrix(respawn_position_);
	}

	void PlayerCharacter::ResetEnergy()
	{
		current_energy_ = 20.0f;
		pickup_counter_ = 0.0f;
		state_ = pickup;
	}

	void PlayerCharacter::Kill()
	{
		dead_counter_ = 0.0f;
		state_ = dead;
	}
}
