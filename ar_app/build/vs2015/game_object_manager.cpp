#include "game_object_manager.h"
#include <graphics\mesh.h>

#include <iostream>

namespace hovar
{
	GameObjectManager::GameObjectManager()
	{
		current_parent_ID_ = 0;
		current_parent_marker_ = NULL;
		//overlap_allowance_ = 0.3f;
		//overlap_counter_ = 0.0f;
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

	bool GameObjectManager::PlayerRoadCollision()
	{
		//overlap_counter_ += dt;
		for (std::list<GameObject*>::iterator tracked_object = marker_specific_objects_.begin(); tracked_object != marker_specific_objects_.end(); ++tracked_object)
		{
			// Do sphercial collision test first as it is less expensive - only do detailed collision when the spheres intersect
			if (CollisionSpherical((*tracked_object)->GetCollider(), hovership_->GetCollider()))
			{
				if (CollisionOOBB((*tracked_object)->GetObb(), hovership_->GetObb()))
				{
					return true;
				}
			}
		}
		return false;
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
		current_parent_ID_ = new_owner->GetParentMarkerID();
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

	bool GameObjectManager::CollisionSpherical(gef::MeshInstance* collider_mesh_1, gef::MeshInstance* collider_mesh_2)
	{
		// Calculate the squared distance between the centers of both spheres
		gef::Vector4 collider_1_pos = collider_mesh_1->mesh()->bounding_sphere().position().Transform(collider_mesh_1->transform());
		gef::Vector4 collider_2_pos = collider_mesh_2->mesh()->bounding_sphere().position().Transform(collider_mesh_2->transform());
		float distance_squared = gef::Vector4(collider_1_pos - collider_2_pos).LengthSqr();

		// Calculate the squared sum of both radii
		float radii_sum_squared = collider_mesh_1->mesh()->bounding_sphere().radius() + collider_mesh_2->mesh()->bounding_sphere().radius();
		radii_sum_squared *= radii_sum_squared;

		// if the the distance squared is less than or equal to the square sum of the radii, then there is a collision
		if (distance_squared <= radii_sum_squared)
		{
			return true;
		}

		// if the distance squared is greater than the sum of the radii squared there is no collision
		return false;
	}

	bool GameObjectManager::CollisionOOBB(obb::OBB * collider_obb_1, obb::OBB * collider_obb_2)
	{
		if (collider_obb_1->OBBOverlap(
			collider_obb_1->E,
			collider_obb_1->O,
			collider_obb_1->GetBasis(),

			collider_obb_2->E,
			collider_obb_2->O,
			collider_obb_2->GetBasis()))
		{
			return true;
		}
		return false;
	}
}

