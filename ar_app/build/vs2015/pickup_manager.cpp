#include "pickup_manager.h"
#include <graphics\mesh.h>
#include <cstdlib>
#include <ctime>
#include <iostream>

namespace hovar
{
	PickupManager::PickupManager()
	{
		//spawn_frequency_ = 5.0f;
		pickups_per_spawn_ = 2;
		spawn_radius_ = 0.2f;
		//spawn_counter_ = 0.0f;
		ready_to_spawn_ = true;
		srand(time(NULL));
	}

	PickupManager::~PickupManager()
	{
		for (std::list<Pickup*>::iterator pickup = active_pickups_.begin(); pickup != active_pickups_.end(); ++pickup)
		{
			delete (*pickup);
			(*pickup) = NULL;
		}
		active_pickups_.clear();
	}

	void PickupManager::Init(GameObjectManager * game_object_manager, gef::Mesh* pickup_mesh_object, gef::Mesh* collider_mesh_object, float mv_ref_size, float pickup_ref_size)
	{
		mv_scale_ = mv_ref_size;
		pickup_scale_ = pickup_ref_size;
		object_manager_ = game_object_manager;
		pickup_mesh_ = pickup_mesh_object;
		collider_mesh_ = collider_mesh_object;
		SpawnNewPickups();
	}

	void PickupManager::Update(float dt)
	{
		for (std::list<Pickup*>::iterator pickup = active_pickups_.begin(); pickup != active_pickups_.end(); ++pickup)
		{
			(*pickup)->Update(dt);
		}
		if (ready_to_spawn_)
		{
			SpawnNewPickups();
			ready_to_spawn_ = false;
		}
	}

	bool PickupManager::PlayerPickupCollision(PlayerCharacter * player)
	{
		for (std::list<Pickup*>::iterator pickup = active_pickups_.begin(); pickup != active_pickups_.end(); ++pickup)
		{
			// if the pickup has not been collected
			if (!(*pickup)->IsCollected())
			{
				// player has collected pickup
				// Do sphercial collision test first as it is less expensive - only do detailed collision when the spheres intersect
				if (CollisionSpherical(player->GetCollider(), (*pickup)->GetCollider()))
				{
					if (CollisionOOBB(player->GetObb(), (*pickup)->GetObb()))
					{
						ready_to_spawn_ = true;
						player->GiveEnergy((*pickup)->Energy());
						(*pickup)->SetCollected(true);
						return true;
					}
				}
			}
		}
		return false;
	}

	void PickupManager::SpawnNewPickups()
	{
		CleanupActivePickups();
		for (int p = 0; p < pickups_per_spawn_; p++)
		{
			Pickup* pickup = CreatePickup();
			object_manager_->TrackObject(pickup);
			active_pickups_.push_back(pickup);
		}
	}

	void PickupManager::CleanupActivePickups()
	{
		for (std::list<Pickup*>::iterator pickup = active_pickups_.begin(); pickup != active_pickups_.end(); ++pickup)
		{
			object_manager_->StopTrackingObject(*pickup);
			(*pickup)->~Pickup();
			delete (*pickup);
		}
		active_pickups_.clear();
	}

	Pickup * PickupManager::CreatePickup()
	{
		gef::Transform pickup_transform;
		pickup_transform.set_scale(gef::Vector4(mv_scale_ * pickup_scale_, mv_scale_ * pickup_scale_, mv_scale_ * pickup_scale_));
		pickup_transform.set_rotation_eulers(gef::Vector4(90.0f, 0.0f, 0.0f));
		pickup_transform.set_translation(gef::Vector4(0.0f, 0.0f, 0.0f));

		gef::Matrix44 mesh_transform;

		Pickup* new_pickup_ = new Pickup();
		new_pickup_->set_mesh(pickup_mesh_);
		mesh_transform.SetIdentity();
		mesh_transform.SetTranslation(gef::Vector4(0.0f, 0.0f, 0.005f));
		new_pickup_->SetCollider(collider_mesh_, mesh_transform, gef::Vector4(mv_scale_ * pickup_scale_ * 42.0f, mv_scale_ * pickup_scale_ * 42.0f, mv_scale_ * pickup_scale_ * 42.0f), "pickup");
		new_pickup_->SetMvTransform(pickup_transform);
		mesh_transform.SetIdentity();
		gef::Vector4 pos = GetSpawnLocation(spawn_radius_, spawn_radius_);
		pos.set_z(mv_scale_ * 80.0f);
		mesh_transform.SetTranslation(pos);
		new_pickup_->SetLocalTransformFromMatrix(mesh_transform);
		new_pickup_->SetParentMarkerID(object_manager_->ParentID());
		return new_pickup_;
	}

	gef::Vector4 PickupManager::GetSpawnLocation(float x_dimensions, float y_dimensions)
	{
		gef::Vector4 spawn_pos;
		spawn_pos.set_x(-x_dimensions + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (x_dimensions + x_dimensions))));
		spawn_pos.set_y(-y_dimensions + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (y_dimensions + y_dimensions))));
		spawn_pos.set_z(0.02f);
		return spawn_pos;
	}

	bool PickupManager::CollisionSpherical(gef::MeshInstance* collider_mesh_1, gef::MeshInstance* collider_mesh_2)
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

	bool PickupManager::CollisionOOBB(obb::OBB * collider_obb_1_, obb::OBB * collider_obb_2_)
	{
		if (collider_obb_1_->OBBOverlap(
			collider_obb_1_->E,
			collider_obb_1_->O,
			collider_obb_1_->GetBasis(),

			collider_obb_2_->E,
			collider_obb_2_->O,
			collider_obb_2_->GetBasis()))
		{
			std::cout << collider_obb_1_->O.x << " " << collider_obb_1_->O.y << " " << collider_obb_1_->O.z << " size " << collider_obb_1_->E.x << " " << collider_obb_1_->E.y << " " << collider_obb_1_->E.z << std::endl;
			std::cout << collider_obb_2_->O.x << " " << collider_obb_2_->O.y << " " << collider_obb_2_->O.z << " size " << collider_obb_2_->E.x << " " << collider_obb_2_->E.y << " " << collider_obb_2_->E.z << std::endl << std::endl;
			return true;
		}
	}
}

