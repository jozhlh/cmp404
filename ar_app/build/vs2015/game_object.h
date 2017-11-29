#ifndef _GAME_OBJECT_H
#define _GAME_OBJECT_H

#include <graphics\mesh_instance.h>
#include <maths\transform.h>
#include <input\sony_controller_input_manager.h>
#include <graphics/renderer_3d.h>
#include <graphics\model.h>
#include <list>
#include "obb.h"

#define NUM_OF_MARKERS 6

//class Platform;

class GameObject : public gef::MeshInstance
{
public:
	GameObject();
	virtual ~GameObject();
	void SetMarkerVisiblity(bool visible) { parent_visible_ = visible; }
	void SetMarkerPosition(gef::Matrix44 marker_transform) { parent_transform_ = marker_transform; }
	int GetParentMarker() { return parent_marker; }
	void SetParentMarker(int new_parent) { parent_marker = new_parent; }
	void UpdateVelocity();
	void VelocityDrag(float dead_zone);
	void VelocityLimits(float max_speed);
	void ApplyAcceleration(gef::Vector4 acceleration) { velocity_ += acceleration; }
	virtual void UpdateMeshTransform();
	virtual void Update();
	void Translate(gef::Vector4 translationVector);
	void Rotate(gef::Vector4 eulerRotation) { m_transform_->Rotate(eulerRotation); UpdateMeshTransform(); }
	void SetVelocity(gef::Vector4 vel) { velocity_ = vel; }
	void SetDrag(float new_drag) { drag_ = new_drag; }
	void SetMvTransform(gef::Transform mvtransform) { m_mv_transform_ = mvtransform; }
	void SetCollider(gef::Mesh* collision_mesh, gef::Matrix44 collider_transform, gef::Vector4 collider_size, std::string name);
	void Stop() { velocity_ = velocity_.kZero; }
	gef::Matrix44 ParentTransform() { return parent_transform_; }
	gef::MeshInstance* Collider() { return collider_; }
	obb::OBB* Obb() { return obb_; }

	gef::Model* Model() { return &m_model_; }
	
	void SetLocalTransformFromMatrix(gef::Matrix44 local_transformation);
	gef::Transform* GetTransform() { return m_transform_; }
	gef::Transform* GetLocalTransform() { return m_local_transform_; }
	gef::Vector4 velocity() { return velocity_; }
	float drag() { return drag_; }
	virtual std::list<gef::MeshInstance*> GetWallCubes();
	virtual std::list<obb::OBB*> GetWallObbs();
	virtual void SetAsParent(bool is_parent) {}
	virtual void Render(gef::Renderer3D* renderer);
protected:
	void SetTransformFromMatrix(gef::Matrix44 transformation_matrix);
	bool parent_visible_;
	gef::Transform* m_transform_;
	gef::Transform m_mv_transform_;
	obb::OBB* obb_;
	gef::MeshInstance* collider_;
	gef::Matrix44 collider_offset;
	gef::Transform* m_local_transform_;
	gef::Matrix44 parent_transform_;
private:
	
	float ApplyDragWithDeadZone(float current_speed, float dead_limits);
	std::string tag;

	gef::Vector4 velocity_;
	
	
	gef::Model m_model_;
	float drag_;

	
	

	int parent_marker;
};

#endif