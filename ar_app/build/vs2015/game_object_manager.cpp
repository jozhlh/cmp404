#include "game_object_manager.h"
#include <graphics\mesh.h>

#include <iostream>

GameObjectManager::GameObjectManager()
{
	current_parent_ = 0;
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
		int parentID = (*tracked_object)->GetParentMarker();
		(*tracked_object)->SetMarkerPosition(marker_transform_matrices_[parentID]);
		(*tracked_object)->SetMarkerVisiblity(markers_visible_[parentID]);
	}
}

void GameObjectManager::TransferOwnership(int new_owner)
{
	current_parent_ = new_owner;
	for (std::list<GameObject*>::iterator tracked_object = marker_bound_objects_.begin(); tracked_object != marker_bound_objects_.end(); ++tracked_object)
	{
		(*tracked_object)->SetParentMarker(new_owner);
	}
}

// Check Player Wall Collisions

// Check Player Road Collisions

bool GameObjectManager::PlayerRoadCollision()
{
	// if player collision with parent road collider
	for (std::list<GameObject*>::iterator tracked_object = marker_specific_objects_.begin(); tracked_object != marker_specific_objects_.end(); ++tracked_object)
	{
		if ((*tracked_object)->GetParentMarker() == hovership_->GetParentMarker())
		{
			//if (CollisionAABB((*tracked_object)->Collider(), hovership_->Collider()))
			if (CollisionOOBB((*tracked_object)->Obb(), hovership_->Obb(), true))
			{
				return true;
			}
			else
			{
				break;
			}
		}
	}
		
	// else if player collision with any other road collider
	for (std::list<GameObject*>::iterator tracked_object = marker_specific_objects_.begin(); tracked_object != marker_specific_objects_.end(); ++tracked_object)
	{
		//if (CollisionSpherical((*tracked_object)->Collider(), hovership_->Collider()))
		if (CollisionOOBB((*tracked_object)->Obb(), hovership_->Obb(), false))
		{
			// transfer all ownership to new parent
			TransferOwnership((*tracked_object)->GetParentMarker());
			return true;
		}
	}
		
	// player is dead
	return false;
}

bool GameObjectManager::PlayerWallCollision(gef::Vector4 * collision_vector)
{
	for (std::list<GameObject*>::iterator tracked_object = marker_specific_objects_.begin(); tracked_object != marker_specific_objects_.end(); ++tracked_object)
	{
		// for each box collider
		bool oldCol = false;
		if (oldCol)
		{
			std::list<gef::MeshInstance*> walls = (*tracked_object)->GetWallCubes();
			for (std::list<gef::MeshInstance*>::iterator wall = walls.begin(); wall != walls.end(); ++wall)
			{
				if (CollisionSpherical(hovership_->Collider(), *wall))
				{
					GetCollisionVector(hovership_->Collider(), *wall, collision_vector);
					return true;
				}
			}
		}
		else
		{
			std::list<obb::OBB*> walls = (*tracked_object)->GetWallObbs();
			std::list<gef::MeshInstance*> wall_meshes = (*tracked_object)->GetWallCubes();
			int it = 0;
			for (std::list<obb::OBB*>::iterator wall = walls.begin(); wall != walls.end(); ++wall)
			{
				if (CollisionOOBB(hovership_->Obb(), *wall, false))
				{
					std::list<gef::MeshInstance*>::iterator wall_mesh_it = wall_meshes.begin();
					std::advance(wall_mesh_it, it);

					GetCollisionVector(hovership_->Collider(), *wall_mesh_it, collision_vector);
					return true;
				}
				it++;
			}
		}
	}

	return false;
}

void GameObjectManager::GetCollisionVector(gef::MeshInstance * collider_mesh_1_, gef::MeshInstance * collider_mesh_2_, gef::Vector4* collision_vector)
{
	collision_vector->set_x(0.0f);
	collision_vector->set_y(0.0f);
	collision_vector->set_z(0.0f);

	// Calculate Collision Vector
	// spherical_normal = player_mesh_transform - box_mesh_transform
	gef::Vector4 spherical_normal = collider_mesh_2_->transform().GetTranslation() - collider_mesh_1_->transform().GetTranslation();
	// spherical_normal.y = 0
	spherical_normal.set_z(0.0f);
	// spherical_normal.normalise
	spherical_normal.Normalise();

	if (abs(spherical_normal.x()) < abs(spherical_normal.y()))
	{
		// collision normal is z axis
		if (spherical_normal.y() > 0.0f)
		{
			collision_vector->set_y(1.0f);
			return;
		}
		else
		{
			collision_vector->set_y(-1.0f);
			return;
		}
	}
	else
	{
		// collision normal is x axis
		if (spherical_normal.x() > 0.0f)
		{
			collision_vector->set_x(1.0f);
			return;
		}
		else
		{
			collision_vector->set_x(-1.0f);
			return;
		}
	}
}

bool GameObjectManager::CollisionAABB(gef::MeshInstance* collider_mesh_1_, gef::MeshInstance* collider_mesh_2_)
{
	// Get the transformed min and max vertices
	gef::Vector4 min_1 = collider_mesh_1_->mesh()->aabb().min_vtx().Transform(collider_mesh_1_->transform());
	gef::Vector4 max_1 = collider_mesh_1_->mesh()->aabb().max_vtx().Transform(collider_mesh_1_->transform());
	gef::Vector4 min_2 = collider_mesh_2_->mesh()->aabb().min_vtx().Transform(collider_mesh_2_->transform());
	gef::Vector4 max_2 = collider_mesh_2_->mesh()->aabb().max_vtx().Transform(collider_mesh_2_->transform());

	return(max_1.x() > min_2.x()
		&& min_1.x() < max_2.x()
		&& max_1.y() > min_2.y()
		&& min_1.y() < max_2.y()
		&& max_1.z() > min_2.z()
		&& min_1.z() < max_2.z());
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

bool GameObjectManager::CollisionOOBB(obb::OBB * collider_obb_1_, obb::OBB * collider_obb_2_, bool debug)
{
	if (collider_obb_1_->OBBOverlap(
		collider_obb_1_->E,
		collider_obb_1_->O,
		collider_obb_1_->GetBasis(),

		collider_obb_2_->E,
		collider_obb_2_->O,
		collider_obb_2_->GetBasis()))
	{
		if (debug)
		{
			std::cout << collider_obb_1_->O.x << " " << collider_obb_1_->O.y << " " << collider_obb_1_->O.z << " size " << collider_obb_1_->E.x << " " << collider_obb_1_->E.y << " " << collider_obb_1_->E.z << std::endl;
			std::cout << collider_obb_2_->O.x << " " << collider_obb_2_->O.y << " " << collider_obb_2_->O.z << " size " << collider_obb_2_->E.x << " " << collider_obb_2_->E.y << " " << collider_obb_2_->E.z << std::endl << std::endl;
		}
		 
		return true;
	}
	return false;
}


