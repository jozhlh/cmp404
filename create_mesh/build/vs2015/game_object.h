#ifndef _GAME_OBJECT_H
#define _GAME_OBJECT_H

#include <graphics\mesh_instance.h>
#include <maths\transform.h>
#include <input\sony_controller_input_manager.h>

class GameObject : public gef::MeshInstance
{
public:
	GameObject();
	~GameObject();
	void UpdateVelocity();
	void UpdateMeshTransform();
	void Translate(gef::Vector4 translationVector);
	void Rotate(gef::Vector4 eulerRotation) { m_transform_->Rotate(eulerRotation); UpdateMeshTransform(); }
	void SetVelocity(gef::Vector4 vel) { velocity_ = vel; }
	gef::Transform* GetTransform() { return m_transform_; }
	gef::Vector4 velocity() { return velocity_; }
	

private:
	gef::Vector4 velocity_;
	gef::Transform* m_transform_;

};

#endif