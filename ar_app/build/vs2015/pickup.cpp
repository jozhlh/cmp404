/*
	pickup.cpp

	Collectable items which recharge the player.

	@author	Josh Hale
	Last Edited: 02/12/17
*/

#include "pickup.h"

namespace hovar
{
	Pickup::Pickup()
	{
		collected_ = false;
		energy_ = 20.0f;
	}


	Pickup::~Pickup()
	{
	}

	void Pickup::Update(float dt)
	{
		GameObject::Update();
		// rotate
		GetLocalTransform()->Rotate(gef::Vector4(0.0f, 0.0f, dt * 45.0f));
	}

	void Pickup::Render(gef::Renderer3D * renderer)
	{
		renderer->DrawMesh(*this);
		//renderer->DrawMesh(*collider_);
	}
}