#include "player_character.h"
#include <iostream>
#include "graphics\mesh.h"
#define STICK_DEAD_ZONE 0.05f

PlayerCharacter::PlayerCharacter()
{
	GameObject();
	max_speed = 0.01f;
	acceleration = 0.0001f;
	deceleration = -0.0001f;
	
	turning_speed = 3.0f;
	SetVelocity(gef::Vector4(0.0f, 0.0f, 0.0f));
	SetDrag(0.000025f);

	current_energy = 20.0f;
	energy_decay_ps = 1.0f;
}

PlayerCharacter::~PlayerCharacter()
{
	delete collider_;
	collider_ = NULL;
}


gef::Vector4 PlayerCharacter::Input(const gef::SonyController* controller_, float dt)
{
	//controller_->UpdateButtonStates(controller_->buttons_down());
	float forward_acceleration = 0.0f;

	// Apply acceleration/deceleration to velocity
	if (controller_->buttons_down() & gef_SONY_CTRL_CROSS)
	{
		//std::cout << "x pressed" << std::endl;
		forward_acceleration = acceleration;
	}
	else if (controller_->buttons_down() & gef_SONY_CTRL_SQUARE)
	{
		//std::cout << "square down" << std::endl;
		forward_acceleration = deceleration;
	}

	if (controller_->buttons_released() & gef_SONY_CTRL_TRIANGLE)
	{
		std::cout << "tri released" << std::endl;
	}

	// left stick sets rotation
	float amountToRotate = 0.0f;
	if ((controller_->left_stick_x_axis() > STICK_DEAD_ZONE) || (controller_->left_stick_x_axis() < STICK_DEAD_ZONE))
	{
		amountToRotate = controller_->left_stick_x_axis() * turning_speed;
		GetLocalTransform()->Rotate(gef::Vector4(0.0f, 0.0f, -1 * amountToRotate));
	}

	// apply acceleration to forward vetor
	gef::Vector4 input_velocity = GetLocalTransform()->GetMatrix().GetColumn(1);

	input_velocity.set_x(-1 * input_velocity.x() * forward_acceleration);
	input_velocity.set_y(input_velocity.y() * forward_acceleration);

	return input_velocity;
}

void PlayerCharacter::Update(const gef::SonyController* controller_, float dt)
{
	current_energy -= (energy_decay_ps * dt);
	if (current_energy < 0.0f)
	{
		// Player Is Dead.
	}
	// Get input
	ApplyAcceleration(Input(controller_, dt));
	// Apply drag to velocity
	VelocityDrag(0.000002f);
	VelocityLimits(max_speed);
	// Update position based on velocity
	UpdateVelocity();
	// Perform collision detection
		// If collision apply reflection velocity
		// Update position based on velocity
}

void PlayerCharacter::UpdateMeshTransform()
{
	transform_ = m_mv_transform_.GetMatrix() * m_transform_->GetMatrix();
	collider_->set_transform(collider_offset * m_transform_->GetMatrix());
	if (obb_ != NULL)
	{
		obb_->SetCoordinateFrameFromMatrix(collider_offset * m_transform_->GetMatrix(), "player");
	}
}

void PlayerCharacter::Render(gef::Renderer3D * renderer)
{
	if (parent_visible_)
	{
		renderer->DrawMesh(*this);
		renderer->DrawMesh(*collider_);
	}
}

void PlayerCharacter::Rebound(gef::Vector4 collision_normal)
{
	gef::Vector4 resultant_velocity = gef::Vector4(0.0f, 0.0f, 0.0f);
	//resultant_velocity.set_x(velocity().x() * collision_normal.x());
	//resultant_velocity.set_y(velocity().y() * collision_normal.y());
	//r = d−2(dDOTn)n
	float dotby2 = 2 * velocity().DotProduct(collision_normal);
	resultant_velocity = velocity();
	resultant_velocity.set_x(resultant_velocity.x() - (dotby2 * collision_normal.x()));
	resultant_velocity.set_y(resultant_velocity.y() - (dotby2 * collision_normal.y()));
	resultant_velocity.set_z(velocity().z() * collision_normal.z());
	resultant_velocity = gef::Vector4(0.0f, 0.0f, 0.0f);
	SetVelocity(resultant_velocity);
	UpdateVelocity();
}

void PlayerCharacter::Respawn()
{
	SetParentMarker(0);
	SetVelocity(gef::Vector4(0.0f, 0.0f, 0.0f));
	SetLocalTransformFromMatrix(respawn_position);
}
