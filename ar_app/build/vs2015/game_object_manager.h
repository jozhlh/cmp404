#ifndef _GAME_OBJECT_MANAGER_H
#define _GAME_OBJECT_MANAGER_H

#include "game_object.h"
#include <maths/matrix44.h>
#include <list>
#include <graphics/renderer_3d.h>
#include <graphics/mesh_instance.h>


class GameObjectManager
{
public:
	GameObjectManager();
	~GameObjectManager();

	void GivePlayerReference(GameObject* new_object);
	void TrackObject(GameObject* new_object) { marker_bound_objects_.push_back(new_object); }
	void StopTrackingObject(GameObject* object_to_ignore) { marker_bound_objects_.remove(object_to_ignore); }
	void AddMarkerSpecificObject(GameObject* new_object) { marker_specific_objects_.push_back(new_object); }
	void UpdateMarkerData();
	void RenderObjects(gef::Renderer3D* renderer);
	void SetMarkerVisiblity(int markerID, bool visible) { markers_visible_[markerID] = visible; }
	void SetMarkerPosition(int markerID, gef::Matrix44 marker_transform) { marker_transform_matrices_[markerID] = marker_transform; }
	bool PlayerRoadCollision();
	bool PlayerWallCollision(gef::Vector4* collision_vector);
	int ParentID() { return current_parent_; }

private:
	void UpdateObjectsInList(std::list<GameObject*> target_list);
	void TransferOwnership(int new_owner);

	void GetCollisionVector(gef::MeshInstance* collider_mesh_1_, gef::MeshInstance* collider_mesh_2_, gef::Vector4* collision_vector);
	bool CollisionAABB(gef::MeshInstance* collider_mesh_1_, gef::MeshInstance* collider_mesh_2_);
	bool CollisionSpherical(gef::MeshInstance* collider_mesh_1_, gef::MeshInstance* collider_mesh_2_);
	bool CollisionOOBB(obb::OBB * collider_obb_1_, obb::OBB * collider_obb_2_, bool debug);

	GameObject* hovership_;
	std::list<GameObject*> marker_bound_objects_;
	std::list<GameObject*> marker_specific_objects_;
	gef::Matrix44 marker_transform_matrices_[NUM_OF_MARKERS];
	bool markers_visible_[NUM_OF_MARKERS];

	int current_parent_;
};

#endif // !_GAME_OBJECT_MANAGER_H


