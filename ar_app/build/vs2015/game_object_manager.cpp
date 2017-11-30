#include "game_object_manager.h"
#include <graphics\mesh.h>

#include <iostream>

namespace hovar
{
	GameObjectManager::GameObjectManager()
	{
		current_parent_ = 0;
		current_parent_marker_ = NULL;
		overlap_allowance_ = 0.3f;
		overlap_counter_ = 0.0f;
		for (int marker_num = 0; marker_num < NUM_OF_MARKERS; marker_num++)
		{
			marker_transform_matrices_[marker_num].SetIdentity();
			markers_visible_[marker_num] = false;
		}
	}

	GameObjectManager::~GameObjectManager()
	{
	}

	void GameObjectManager::GivePlayerReference(GameObject * new_object)
	{
		hovership_ = new_object;
		TrackObject(new_object);
	}

	void GameObjectManager::UpdateMarkerData()
	{
		UpdateObjectsInList(marker_bound_objects_);
		UpdateObjectsInList(marker_specific_objects_);
	}

	void GameObjectManager::RenderObjects(gef::Renderer3D * renderer)
	{
		for (std::list<GameObject*>::iterator tracked_object = marker_bound_objects_.begin(); tracked_object != marker_bound_objects_.end(); ++tracked_object)
		{
			(*tracked_object)->Render(renderer);
		}
		for (std::list<GameObject*>::iterator tracked_object = marker_specific_objects_.begin(); tracked_object != marker_specific_objects_.end(); ++tracked_object)
		{
			(*tracked_object)->Render(renderer);
		}
	}

	void GameObjectManager::UpdateObjectsInList(std::list<GameObject*> target_list)
	{
		for (std::list<GameObject*>::iterator tracked_object = target_list.begin(); tracked_object != target_list.end(); ++tracked_object)
		{
			int parentID = (*tracked_object)->GetParentMarkerID();
			(*tracked_object)->SetMarkerPosition(marker_transform_matrices_[parentID]);
			(*tracked_object)->SetMarkerVisiblity(markers_visible_[parentID]);
		}
	}

	void GameObjectManager::TransferOwnership(GameObject* new_owner)
	{
		current_parent_ = new_owner->GetParentMarkerID();
		for (std::list<GameObject*>::iterator tracked_object = marker_bound_objects_.begin(); tracked_object != marker_bound_objects_.end(); ++tracked_object)
		{
			SetNewLocal((*tracked_object), new_owner);
			(*tracked_object)->SetParentMarkerID(new_owner->GetParentMarkerID());
		}
	}

	void GameObjectManager::SetNewLocal(GameObject * current_object, GameObject * new_parent)
	{
		gef::Transform* object_world = current_object->GetTransform();
		gef::Transform* new_parent_transform = new_parent->GetTransform();

		gef::Matrix44 inv;
		inv.Inverse(new_parent_transform->GetMatrix());

		current_object->SetMarkerPosition(new_parent_transform->GetMatrix());
		current_object->SetLocalTransformFromMatrix(object_world->GetMatrix() * inv);
	}

	void GameObjectManager::FindNewParent()
	{
		for (std::list<GameObject*>::iterator tracked_object = marker_specific_objects_.begin(); tracked_object != marker_specific_objects_.end(); ++tracked_object)
		{
			if (CollisionOOBB((*tracked_object)->GetObb(), hovership_->GetObb()))
			{
				TransferOwnership(*tracked_object);
				current_parent_marker_ = *tracked_object;
				for (std::list<GameObject*>::iterator segs = marker_specific_objects_.begin(); segs != marker_specific_objects_.end(); ++segs)
				{
					(*segs)->SetAsParent(false);
				}
				(*tracked_object)->SetAsParent(true);
				return;
			}
		}
	}

	bool GameObjectManager::PlayerRoadCollision(float dt)
	{
		overlap_counter_ += dt;
		// if player collision with parent road collider
		for (std::list<GameObject*>::iterator tracked_object = marker_specific_objects_.begin(); tracked_object != marker_specific_objects_.end(); ++tracked_object)
		{
			if (CollisionOOBB((*tracked_object)->GetObb(), hovership_->GetObb()))
			{
				return true;
			}
		}

		// player is dead
		return false;
	}

	bool GameObjectManager::CollisionSpherical(gef::MeshInstance* collider_mesh_1_, gef::MeshInstance* collider_mesh_2_)
	{
		// Calculate the squared distance between the centers of both spheres
		gef::Vector4 collider_1_pos = collider_mesh_1_->mesh()->bounding_sphere().position().Transform(collider_mesh_1_->transform());
		gef::Vector4 collider_2_pos = collider_mesh_2_->mesh()->bounding_sphere().position().Transform(collider_mesh_2_->transform());
		float distance_squared = gef::Vector4(collider_1_pos - collider_2_pos).LengthSqr();

		// Calculate the squared sum of both radii
		float radii_sum_squared = collider_mesh_1_->mesh()->bounding_sphere().radius() + collider_mesh_2_->mesh()->bounding_sphere().radius();
		radii_sum_squared *= radii_sum_squared;

		// if the the distance squared is less than or equal to the square sum of the radii, then there is a collision
		if (distance_squared <= radii_sum_squared)
		{
			return true;
		}

		// if the distance squared is greater than the sum of the radii squared there is no collision
		return false;
	}

	bool GameObjectManager::CollisionOOBB(obb::OBB * collider_obb_1_, obb::OBB * collider_obb_2_)
	{
		if (collider_obb_1_->OBBOverlap(
			collider_obb_1_->E,
			collider_obb_1_->O,
			collider_obb_1_->GetBasis(),

			collider_obb_2_->E,
			collider_obb_2_->O,
			collider_obb_2_->GetBasis()))
		{
			return true;
		}
		return false;
	}
}

