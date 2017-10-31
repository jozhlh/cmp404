#include "pickup.h"



Pickup::Pickup()
{
	collected = false;
	energy = 20.0f;
}


Pickup::~Pickup()
{
}

void Pickup::Update()
{
	GameObject::Update();
	// rotate
}

void Pickup::Render(gef::Renderer3D * renderer)
{
	renderer->DrawMesh(*this);
	if (!collected && parent_visible_)
	{
		
		renderer->DrawMesh(*collider_);
	}
}
