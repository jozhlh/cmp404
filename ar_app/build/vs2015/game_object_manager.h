#ifndef _GAME_OBJECT_MANAGER_H
#define _GAME_OBJECT_MANAGER_H

#include "game_object.h"
#include <maths/matrix44.h>
#include <list>
#include <graphics/renderer_3d.h>
#include <graphics/mesh_instance.h>

namespace hovar
{
	/**
	Tracks all game objects.
	*/
	class GameObjectManager
	{
	public:
		/// @brief Default constructor.
		/// @note Initialises marker data.
		GameObjectManager();

		/// @brief Default destructor.
		/// @note No default functionality.
		~GameObjectManager();

		/// @brief Gives manager reference to player.
		/// @note Tracks the player object.
		/// @param[in] new_object	The player object.
		void GivePlayerReference(GameObject* new_object);

		/// @brief Track game object.
		/// @param[in] new_object	The game object to track.
		void TrackObject(GameObject* new_object) { marker_bound_objects_.push_back(new_object); }
		
		/// @brief Stop tracking game object.
		/// @param[in] object_to_ignore	The game object to stop tracking.
		void StopTrackingObject(GameObject* object_to_ignore) { marker_bound_objects_.remove(object_to_ignore); }
		
		/// @brief Track marker object.
		/// @note Tracks object which is bound to a specific marker.
		/// @param[in] new_object	The game object to track.
		void AddMarkerSpecificObject(GameObject* new_object) { marker_specific_objects_.push_back(new_object); }
		
		/// @brief Update all tracked objects.
		void UpdateMarkerData();

		/// @brief Render all tracked objects.
		/// @param[in] renderer 	The 3D rendering engine.
		void RenderObjects(gef::Renderer3D* renderer);

		/// @brief Set whether a marker is visible.
		/// @param[in] markerID 	The ID of the marker.
		/// @param[in] visible 		Whether the marker is visible.
		void SetMarkerVisiblity(int markerID, bool visible) { markers_visible_[markerID] = visible; }
		
		/// @brief Set a marker's transform.
		/// @param[in] markerID 			The ID of the marker.
		/// @param[in] marker_transform 	The new transform of the marker.
		void SetMarkerPosition(int markerID, gef::Matrix44 marker_transform) { marker_transform_matrices_[markerID] = marker_transform; }
		
		/// @brief Assigns a new reference marker.
		/// @note Finds the first marker object that the player is colliding with and assigns that marker
		/// as the new reference marker.
		void FindNewParent();

		/// @brief Checks whether player is on a road.
		/// @return If the player is currently touching a road.
		bool PlayerRoadCollision(float dt);

		/// @brief Returns ID of reference marker.
		/// @return The ID of the reference marker.
		int ParentID() { return current_parent_; }

	private:
		/// @brief Updates tracked objects.
		/// @note Sends the marker position and visibility to all objects in a tracking list.
		/// @param[in] target_list 	The list of objects being tracked to update.
		void UpdateObjectsInList(std::list<GameObject*> target_list);
		
		/// @brief Set new reference marker.
		/// @note Sets all non marker specific objects to a new reference marker.
		/// @param[in] new_owner 	The new parent marker object.
		void TransferOwnership(GameObject* new_owner);

		/// @brief Set new local transform.
		/// @note Calculates the local transform with resoect to the new parent marker
		/// whilst maintaining the same world position.
		/// @param[in] current_object 	The object which is to have a new local transform.
		/// @param[in] new_parent 		The object which is the new parent.
		void SetNewLocal(GameObject* current_object, GameObject* new_parent);

		/// @brief Spherical collision test.
		/// @note Determines if two spheres are overlapping.
		/// @param[in] collider_mesh_1 	Sphere 1.
		/// @param[in] collider_mesh_2 	Sphere 2.
		/// @return Whether Sphere 1 and Sphere 2 overlap.
		bool CollisionSpherical(gef::MeshInstance* collider_mesh_1, gef::MeshInstance* collider_mesh_2);
		
		/// @brief Oriented bounding box collision test.
		/// @note Determines whether two cubes overlap in 3D space.
		/// @param[in] collider_obb_1 	The first collision volume.
		/// @param[in] collider_obb_2 	The second collision volume.
		/// @return Whether the two collision volumes overlap.
		bool CollisionOOBB(obb::OBB * collider_obb_1, obb::OBB * collider_obb_2);

		/// Reference to the player controlled vehicle
		GameObject* hovership_;

		/// Reference to the current reference marker
		GameObject* current_parent_marker_;

		/// List of tracked objects which can be bound to any marker
		std::list<GameObject*> marker_bound_objects_;

		/// List of tracked objects which are bound to a specific marker
		std::list<GameObject*> marker_specific_objects_;

		// UNUSED ??? ///
		gef::Matrix44 marker_displacement_;

		/// The last known transforms of all markers
		gef::Matrix44 marker_transform_matrices_[NUM_OF_MARKERS];

		/// Each marker's visibility status
		bool markers_visible_[NUM_OF_MARKERS];

		/// Amount of time to wait before allowing reassigning reference marker
		float overlap_allowance_;

		/// Time since reference marker was reassigned
		float overlap_counter_;

		/// ID of the current reference marker
		int current_parent_;
	};
}
#endif // !_GAME_OBJECT_MANAGER_H


