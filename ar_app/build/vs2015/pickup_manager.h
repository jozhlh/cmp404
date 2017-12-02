#ifndef _PICKUP_MANAGER_H
#define _PICKUP_MANAGER_H

#include "pickup.h"
#include <list>
#include <graphics/mesh_instance.h>
#include "player_character.h"
#include "game_object_manager.h"

namespace hovar
{
	/**
	Manages the spawning and collision of collectable items.
	*/
	class PickupManager
	{
	public:
		/// @brief Default constructor.
		/// @note Initialises spawning parameters and seeds random from time.
		PickupManager();

		/// @brief Default destructor.
		/// @note Releases any active pickups.
		~PickupManager();

		/// @brief Initialiser.
		/// @note Stores pickup mesh information and spawns first set of pickups.
		/// @param[in] game_object_manager		The manager which tracks the pickups.
		/// @param[in] pickup_mesh_object		The mesh of the pickup model.
		/// @param[in] collider_mesh_object		The mesh of the collision volume for visualisation.
		/// @param[in] mv_ref_size				The scale factor of the art asset.
		/// @param[in] pickup_ref_size			The scale factor of the pickup object.
		void Init(GameObjectManager* game_object_manager, gef::Mesh* pickup_mesh_object, gef::Mesh* collider_mesh_object, float mv_ref_size, float pickup_ref_size);
		
		/// @brief Updates every frame.
		/// @note Updates all pickups, if waiting to spawn new pickups they are spawned.
		/// @param[in] dt		The time since the last frame (delta time).
		void Update(float dt);

		/// @brief Checks pickup collisions.
		/// @note Checks the player for collision against all active pickups.
		/// Gives the player energy if there was a collision.
		/// @param[in] player		The player character object.
		/// @return True if there was a collision.
		bool PlayerPickupCollision(PlayerCharacter* player);

		/// @brief Gets the model component.
		/// @return The model object.
		gef::Model* GetModel() { return &m_model_; }

		/// @brief Respawns pickups.
		void Reset() { ready_to_spawn_ = true; }

	private:
		/// @brief Spawns new pickups.
		/// @note Spawns the specified number of pickups and sends them to the object manager to be tracked.
		void SpawnNewPickups();

		/// @brief Releases active pickups.
		/// @note Stops objects from being tracked and deletes them.
		void CleanupActivePickups();

		/// @brief Creates a pickup object.
		/// @note Spanws a pickup at a random location.
		/// @return The created pickup.
		Pickup* CreatePickup();
		
		/// @brief Gets a random vector.
		/// @note Uses a specified range and gets a random position in that range.
		/// @param[in] x_dimensions		The x bounds.
		/// @param[in] y_dimensions		The y bounds.
		/// @return The random location.
		gef::Vector4 GetSpawnLocation(float x_dimensions, float y_dimensions);

		/// @brief Oriented bounding box collision test.
		/// @note Determines whether two cubes overlap in 3D space.
		/// @param[in] collider_obb_1 	The first collision volume.
		/// @param[in] collider_obb_2 	The second collision volume.
		/// @return Whether the two collision volumes overlap.
		bool CollisionOOBB(obb::OBB * collider_obb_1_, obb::OBB * collider_obb_2_);
		
		/// The manager which tracks objects
		GameObjectManager* object_manager_;

		/// The list of currently spawned pickups
		std::list<Pickup*> active_pickups_;

		/// The mesh of the pickup model
		gef::Mesh* pickup_mesh_;

		/// The mesh of the collision volume
		gef::Mesh* collider_mesh_;

		/// The pickup model
		gef::Model m_model_;

		/// The art asset scaling factor
		float mv_scale_;

		/// The pickup scaling factor
		float pickup_scale_;

		/// How regularly to automatically spawn objects
		float spawn_frequency_;

		/// How long since pickups were last spawned
		float spawn_counter_;

		/// How far away from the centre point pickups can be spawned
		float spawn_radius_;

		/// How many pickups to spawn at one time
		int pickups_per_spawn_;

		/// Is the manager waiting to spawn new pickups
		bool ready_to_spawn_;
	};
}
#endif

