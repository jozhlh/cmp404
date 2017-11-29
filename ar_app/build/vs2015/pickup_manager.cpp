#include "pickup_manager.h"
#include <graphics\mesh.h>
#include <cstdlib>
#include <ctime>
#include <iostream>

PickupManager::PickupManager()
{
	spawn_frequency = 5.0f;
	pickups_per_spawn = 2;
	spawn_radius = 0.2f;
	spawn_counter = 0.0f;
	ready_to_spawn_ = true;
	srand(time(NULL));
}

PickupManager::~PickupManager()
{
	for (std::list<Pickup*>::iterator pickup = active_pickups.begin(); pickup != active_pickups.end(); ++pickup)
	{
		delete (*pickup);
	}
	active_pickups.clear();
}

void PickupManager::Init(GameObjectManager * game_object_manager, gef::Mesh* pickup_mesh_object, gef::Mesh* collider_mesh_object, float mv_ref_size, float pickup_ref_size)
{
	mv_scale = mv_ref_size;
	pickup_scale = pickup_ref_size;
	object_manager_ = game_object_manager;
	pickup_mesh_ = pickup_mesh_object;
	collider_mesh_ = collider_mesh_object;
	SpawnNewPickups();
}

void PickupManager::Update(float dt)
{
	for (std::list<Pickup*>::iterator pickup = active_pickups.begin(); pickup != active_pickups.end(); ++pickup)
	{
		(*pickup)->Update(dt);
	}
	if (ready_to_spawn_)
	{
		SpawnNewPickups();
		ready_to_spawn_ = false;
	}
}

void PickupManager::CleanupActivePickups()
{
	for (std::list<Pickup*>::iterator pickup = active_pickups.begin(); pickup != active_pickups.end(); ++pickup)
	{
		object_manager_->StopTrackingObject(*pickup);
		(*pickup)->~Pickup();
		delete (*pickup);
	}
	active_pickups.clear();
}

bool PickupManager::CollisionSpherical(gef::MeshInstance* collider_mesh_1_, gef::MeshInstance* collider_mesh_2_)
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

bool PickupManager::CollisionAABB(gef::MeshInstance * collider_mesh_1_, gef::MeshInstance * collider_mesh_2_)
{
	// http://www.dyn4j.org/2010/01/sat/


	//std::cout << "checking pickup collision" << std::endl;
	// Get the transformed min and max vertices
	gef::Vector4 min_1 = collider_mesh_1_->mesh()->aabb().min_vtx().Transform(collider_mesh_1_->transform());
	gef::Vector4 max_1 = collider_mesh_1_->mesh()->aabb().max_vtx().Transform(collider_mesh_1_->transform());
	gef::Vector4 min_2 = collider_mesh_2_->mesh()->aabb().min_vtx().Transform(collider_mesh_2_->transform());
	gef::Vector4 max_2 = collider_mesh_2_->mesh()->aabb().max_vtx().Transform(collider_mesh_2_->transform());

	if (max_1.x() > min_2.x()
		&& min_1.x() < max_2.x()
		&& max_1.y() > min_2.y()
		&& min_1.y() < max_2.y()
		&& max_1.z() > min_2.z()
		&& min_1.z() < max_2.z())
	{
		std::cout << "pickup collision registered" << std::endl;
	}

	return(max_1.x() > min_2.x()
		&& min_1.x() < max_2.x()
		&& max_1.y() > min_2.y()
		&& min_1.y() < max_2.y()
		&& max_1.z() > min_2.z()
		&& min_1.z() < max_2.z());
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

gef::Vector4 PickupManager::GetSpawnLocation(float xDimensions, float yDimensions)
{
	//std::random_device device;
	//std::mt19937 generator(device());
	//std::uniform_real<float> xDistribution(-xDimensions, xDimensions);
	//std::uniform_real<float> yDistribution(-yDimensions, yDimensions);

	gef::Vector4 spawn_pos;
	spawn_pos.set_x(-xDimensions + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (xDimensions + xDimensions))));
	spawn_pos.set_y(-yDimensions + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (yDimensions + yDimensions))));
	spawn_pos.set_z(0.02f);
	//spawn_pos.set_x(0.0f);
	//spawn_pos.set_y(0.0f);
	return spawn_pos;
}

void PickupManager::SpawnNewPickups()
{
	CleanupActivePickups();
	for (int p = 0; p < pickups_per_spawn; p++)
	{
		Pickup* pickup = CreatePickup();
		object_manager_->TrackObject(pickup);
		active_pickups.push_back(pickup);
	}
}

Pickup * PickupManager::CreatePickup()
{
	gef::Transform pickup_transform;
	pickup_transform.set_scale(gef::Vector4(mv_scale * pickup_scale, mv_scale * pickup_scale, mv_scale * pickup_scale));
	pickup_transform.set_rotation_eulers(gef::Vector4(90.0f, 0.0f, 0.0f));
	pickup_transform.set_translation(gef::Vector4(0.0f, 0.0f, 0.0f));

	gef::Matrix44 mesh_transform;

	Pickup* new_pickup_ = new Pickup();
	new_pickup_->set_mesh(pickup_mesh_);
	mesh_transform.SetIdentity();
	mesh_transform.SetTranslation(gef::Vector4(0.0f, 0.0f, 0.01f));
	new_pickup_->SetCollider(collider_mesh_, mesh_transform, gef::Vector4(mv_scale * pickup_scale * 42.0f, mv_scale * pickup_scale * 42.0f, mv_scale * pickup_scale * 42.0f), "pickup");
	new_pickup_->SetMvTransform(pickup_transform);
	mesh_transform.SetIdentity();
	gef::Vector4 pos = GetSpawnLocation(spawn_radius, spawn_radius);
	mesh_transform.SetTranslation(pos);
	new_pickup_->SetLocalTransformFromMatrix(mesh_transform);
	new_pickup_->SetParentMarker(object_manager_->ParentID());
	return new_pickup_;
}

bool PickupManager::PlayerPickupCollision(PlayerCharacter * player)
{
	for (std::list<Pickup*>::iterator pickup = active_pickups.begin(); pickup != active_pickups.end(); ++pickup)
	{
		if (!(*pickup)->IsCollected())
		{
			/*
			if (CollisionSpherical(player->Collider(), (*pickup)->Collider()))
			{
				std::cout << "pickup collision" << std::endl;
				// player has collected pickup
				player->GiveEnergy((*pickup)->Energy());
				(*pickup)->SetCollected(true);
			}*/
			if (CollisionOOBB(player->Obb(), (*pickup)->Obb()))
			{
				ready_to_spawn_ = true;
				std::cout << "pickup collision" << std::endl;
				// player has collected pickup
				player->GiveEnergy((*pickup)->Energy());
				(*pickup)->SetCollected(true);
				return true;
			}
		}
	}
	return false;
}


