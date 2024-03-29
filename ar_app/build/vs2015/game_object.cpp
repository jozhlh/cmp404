/*
	game_object.cpp

	The default functionality of all objects in the game.

	@author	Josh Hale
	Last Edited: 02/12/17
*/

#include "game_object.h"

namespace hovar
{
	GameObject::GameObject()
	{
		m_transform_ = new gef::Transform();
		m_local_transform_ = new gef::Transform();
		collider_offset_.SetIdentity();
		parent_transform_.SetIdentity();
		parent_visible_ = false;
		parent_marker_id_ = 1;
	}

	GameObject::~GameObject()
	{
		delete m_transform_;
		m_transform_ = NULL;

		delete m_local_transform_;
		m_local_transform_ = NULL;
	}

	void GameObject::UpdateMeshTransform()
	{
		transform_ = m_mv_transform_.GetMatrix() * m_transform_->GetMatrix();
		if (collider_ != NULL)
		{
			collider_->set_transform(collider_offset_ * m_transform_->GetMatrix());
		}
		if (obb_ != NULL)
		{
			obb_->SetCoordinateFrameFromMatrix(collider_offset_ * m_transform_->GetMatrix(), tag_);
		}
	}

	void GameObject::Update()
	{
		SetTransformFromMatrix(m_local_transform_->GetMatrix() * parent_transform_);
	}

	void GameObject::Render(gef::Renderer3D * renderer)
	{
		if (parent_visible_)
		{
			renderer->DrawMesh(*this);
		}
	}

	void GameObject::UpdateVelocity()
	{
		gef::Vector4 new_position = m_local_transform_->translation();
		m_local_transform_->set_translation(new_position += velocity_);
		SetTransformFromMatrix(m_local_transform_->GetMatrix() * parent_transform_);
	}

	void GameObject::VelocityDrag()
	{
		gef::Vector4 drag_vector = velocity_;
		velocity_.set_x(ApplyDragWithDeadZone(drag_vector.x(), drag_dead_zone_));
		velocity_.set_y(ApplyDragWithDeadZone(drag_vector.y(), drag_dead_zone_));
	}

	void GameObject::VelocityLimits(float max_speed)
	{
		if (velocity_.Length() > max_speed)
		{
			velocity_.Normalise();
			velocity_ *= max_speed;
		}
	}

	void GameObject::Rotate(gef::Vector4 euler_rotation)
	{
		m_transform_->Rotate(euler_rotation);
		UpdateMeshTransform();
	}

	void GameObject::SetLocalTransformFromMatrix(gef::Matrix44 transformation_matrix)
	{
		m_local_transform_->Set(transformation_matrix);
		SetTransformFromMatrix(m_local_transform_->GetMatrix() * parent_transform_);
	}

	void GameObject::SetCollider(gef::Mesh* collision_mesh, gef::Matrix44 collider_transform, gef::Vector4 collider_size, std::string name)
	{
		collider_ = new gef::MeshInstance();
		collider_->set_mesh(collision_mesh);

		collider_offset_ = collider_transform;

		collider_->set_transform(collider_offset_);

		tag_ = name;

		obb_ = new obb::OBB(obb::Vector(0.5f * collider_size.x(), 0.5f * collider_size.z(), 0.5f * collider_size.y()));
		obb_->SetCoordinateFrameFromMatrix(collider_offset_, tag_);
	}

	void GameObject::SetTransformFromMatrix(gef::Matrix44 transformation_matrix)
	{
		m_transform_->Set(transformation_matrix);
		UpdateMeshTransform();
	}

	float GameObject::ApplyDragWithDeadZone(float current_speed, float dead_limits)
	{
		if (current_speed > dead_limits)
		{
			current_speed -= drag_;
		}
		else if (current_speed < (-1 * dead_limits))
		{
			current_speed += drag_;
		}

		if ((current_speed <= dead_limits) && (current_speed >= (-1 * dead_limits)))
		{
			current_speed = 0.0f;
		}
		return current_speed;
	}
}

