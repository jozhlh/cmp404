#ifndef _PICKUP_MANAGER_H
#define _PICKUP_MANAGER_H

#include "pickup.h"
#include <list>
#include <graphics/mesh_instance.h>
#include "player_character.h"
#include "game_object_manager.h"

class PickupManager
{
public:
	PickupManager();
	~PickupManager();

	void Init(GameObjectManager* game_object_manager, gef::Mesh* pickup_mesh_object, gef::Mesh* collider_mesh_object, float mv_ref_size, float pickup_ref_size);
	void Update(float dt);
	void SpawnNewPickups();
	Pickup* CreatePickup();
	void PlayerPickupCollision(PlayerCharacter* player);
	gef::Model* Model() { return &m_model_; }

private:
	void CleanupActivePickups();
	bool CollisionSpherical(gef::MeshInstance* collider_mesh_1_, gef::MeshInstance* collider_mesh_2_);
	bool CollisionAABB(gef::MeshInstance* collider_mesh_1_, gef::MeshInstance* collider_mesh_2_);
	gef::Vector4 GetSpawnLocation(float xDimensions, float yDimensions);

	GameObjectManager* object_manager_;
	std::list<Pickup*> active_pickups;

	gef::Mesh* pickup_mesh_;
	gef::Mesh* collider_mesh_;

	gef::Model m_model_;

	float mv_scale;
	float pickup_scale;

	float spawn_frequency;
	float spawn_counter;
	int pickups_per_spawn;
};

#endif

