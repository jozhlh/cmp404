#include "game_object.h"

GameObject::GameObject()
{
	m_transform_ = new gef::Transform();
	UpdateMeshTransform();
}

GameObject::~GameObject()
{
	if (m_transform_)
	{
		delete m_transform_;
		m_transform_ = NULL;
	}
}

// Updates the position of the game object
void GameObject::UpdateVelocity()
{
	gef::Vector4 new_position = m_transform_->translation();
	m_transform_->set_translation(new_position += velocity_);
	UpdateMeshTransform();
}

void GameObject::UpdateMeshTransform()
{
	transform_ = m_transform_->GetMatrix();
}

void GameObject::Translate(gef::Vector4 translationVector)
{
	gef::Vector4 new_position = m_transform_->translation();
	m_transform_->set_translation(new_position += translationVector);
	UpdateMeshTransform();
}
