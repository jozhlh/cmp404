#ifndef _PICKUP_MANAGER_H
#define _PICKUP_MANAGER_H

#include "pickup.h"
#include <list>
#include <graphics/mesh_instance.h>
#include "player_character.h"
#include "game_object_manager.h"

namespace hovar
{
	class PickupManager
	{
	public:
		PickupManager();
		~PickupManager();

		void Init(GameObjectManager* game_object_manager, gef::Mesh* pickup_mesh_object, gef::Mesh* collider_mesh_object, float mv_ref_size, float pickup_ref_size);
		void Update(float dt);
		bool PlayerPickupCollision(PlayerCharacter* player);
		gef::Model* GetModel() { return &m_model_; }
		void Reset() { ready_to_spawn_ = true; }

	private:
		void SpawnNewPickups();
		Pickup* CreatePickup();
		void CleanupActivePickups();
		bool CollisionOOBB(obb::OBB * collider_obb_1_, obb::OBB * collider_obb_2_);
		gef::Vector4 GetSpawnLocation(float x_dimensions, float y_dimensions);

		GameObjectManager* object_manager_;
		std::list<Pickup*> active_pickups_;
		gef::Mesh* pickup_mesh_;
		gef::Mesh* collider_mesh_;
		gef::Model m_model_;
		float mv_scale_;
		float pickup_scale_;
		float spawn_frequency_;
		float spawn_counter_;
		float spawn_radius_;
		int pickups_per_spawn_;
		bool ready_to_spawn_;
	};
}
#endif

