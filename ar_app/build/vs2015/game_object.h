#ifndef _GAME_OBJECT_H
#define _GAME_OBJECT_H

#include <graphics\mesh_instance.h>
#include <maths\transform.h>
#include <input\sony_controller_input_manager.h>
#include <graphics/renderer_3d.h>
#include <graphics\model.h>
#include <list>
#include "obb.h"

#define NUM_OF_MARKERS 6

namespace hovar
{
	/**
	The default functionality of all objects in the game.
	*/
	class GameObject : public gef::MeshInstance
	{
	public:
		/// @brief Default constructor.
		/// @note Creates transform components and initializes member variables.
		GameObject();

		/// @brief Virtual constructor.
		/// @note Releases transform components.
		virtual ~GameObject();

		/// @brief Updates transform components.
		/// @note Mesh and collider transforms are updated according to current transform.
		virtual void UpdateMeshTransform();

		/// @brief Updates transform from local transform.
		/// @note Uses local and parent transform to get world transform.
		virtual void Update();

		/// @brief Interface for setting parent marker object.
		/// @note No default functionality.
		/// @param[in]	is_parent	Whether the object's marker is the parent marker.
		virtual void SetAsParent(bool is_parent) {}

		/// @brief Virtual render.
		/// @note Renders the mesh attached to this object.
		/// @param[in]	renderer	The 3D rendering engine.
		virtual void Render(gef::Renderer3D* renderer);

	/* <Move to player character?> */
		/// @brief Updates position.
		/// @note Uses the current velocity to update the position.
		void UpdateVelocity();

		/// @brief Reduces the velocity.
		/// @note Reduces the velocity by the drag value. If the velocity is smaller than
		/// the dead zone, the velocity is set to zero.
		/// @param[in]	dead_zone	The point at which the velocity is assumed to be zero.
		void VelocityDrag(float dead_zone);

		/// @brief Limits the velocity.
		/// @note Stops the velocity going over the max speed.
		/// @param[in]	max_speed	The fastest a game object can move.
		void VelocityLimits(float max_speed);

		/// @brief Adds acceleration to velocity.
		/// @param[in]	acceleration	The amount to increase velocity.
		void ApplyAcceleration(gef::Vector4 acceleration) { velocity_ += acceleration; }

		/// @brief Set the velocity to zero.
		void Stop() { velocity_ = velocity_.kZero; }
	/* </Move to player character?> */
		
		/// @brief Rotates the game object.
		/// @param[in] euler_rotation	The amount to rotate the object.
		void Rotate(gef::Vector4 euler_rotation);
		
		/// @brief Set marker visibility.
		/// @note Set whether the game object's parent marker is visible by the camera.
		/// @param[in] visible	Is the marker visible.
		void SetMarkerVisiblity(bool visible) { parent_visible_ = visible; }
		
		/// @brief Set marker position.
		/// @note Set the parent transform to be that of the parent marker.
		/// @param[in] marker_transform		The parent marker's transform.
		void SetMarkerPosition(gef::Matrix44 marker_transform) { parent_transform_ = marker_transform; }
		
		/// @brief Set the drag value.
		/// @param[in] new_drag		The new drag value.
		void SetDrag(float new_drag) { drag_ = new_drag; }

		/// @brief Set the asset offset.
		/// @note Set the transform that converts the imported asset's transform
		/// to the observed transform of the object.
		/// @param[in] mvtransform		The offset of the imported art asset.
		void SetMvTransform(gef::Transform mvtransform) { m_mv_transform_ = mvtransform; }
		
		/// @brief Set the local transform.
		/// @note Uses the transformation matrix to update the objects local position.
		/// @param[in] local_transformation		The object's new local transform.
		void SetLocalTransformFromMatrix(gef::Matrix44 local_transformation);
		
		/// @brief Returns the local transform.
		/// @return The local transform.
		gef::Transform* GetLocalTransform() { return m_local_transform_; }

		/// @brief Returns the world transform.
		/// @return The world transform.
		gef::Transform* GetTransform() { return m_transform_; }

		/// @brief Returns the parent's transform.
		/// @return The parent's transform.
		gef::Matrix44 GetParentTransform() { return parent_transform_; }

		/// @brief Returns the model object.
		/// @return Reference to the model object.
		gef::Model* GetModel() { return &m_model_; }

		/// @brief Returns the collision volume.
		/// @return The obb collision volume.
		obb::OBB* GetObb() { return obb_; }

		/// @brief Sets the ID of the parent marker.
		/// @param[in] new_parent_id	The ID of the new parent marker.
		void SetParentMarkerID(int new_parent_id) { parent_marker_id_ = new_parent_id; }
		
		/// @brief Returns the parent's ID.
		/// @return The ID of the marker which is currently the parent of this object.
		int GetParentMarkerID() { return parent_marker_id_; }
		
		/// @brief Sets the new velocity.
		/// @param[in] vel	The new velocity value.
		void SetVelocity(gef::Vector4 vel) { velocity_ = vel; }

		/// @brief Returns the velocity.
		/// @return The current velocity.
		gef::Vector4 GetVelocity() { return velocity_; }

		/// @brief Creates a new collider.
		/// @note Creates a collision mesh for visualising the collider. Also creates obb volume
		/// @param[in] collision_mesh	The collider mesh for visualisation.
		/// @param[in] collider_transform	The offset of the collider.
		/// @param[in] collider_size	The size of the collider.
		/// @param[in] name	The tag of the collider.
		void SetCollider(gef::Mesh* collision_mesh, gef::Matrix44 collider_transform, gef::Vector4 collider_size, std::string name);
		
		/// @brief Returns the collider mesh.
		/// @return The collider object.
		gef::MeshInstance* GetCollider() { return collider_; }

	protected:
		/// @brief Set the world transform.
		/// @note Uses the transformation matrix to update the objects world position.
		/// @param[in] transformation_matrix		The object's new world transform.
		void SetTransformFromMatrix(gef::Matrix44 transformation_matrix);
		
		/// Is the object's parent marker visible
		bool parent_visible_;

		/// The world transform of the object
		gef::Transform* m_transform_;

		/// The offset of the art asset
		gef::Transform m_mv_transform_;

		/// The obb collision volume
		obb::OBB* obb_;

		/// The collider mesh for visualisation
		gef::MeshInstance* collider_;

		/// The offset of the collision volume
		gef::Matrix44 collider_offset_;

		/// The local transform of the object
		gef::Transform* m_local_transform_;

		/// The world transform of the parent object
		gef::Matrix44 parent_transform_;
	
	private:
	/* <Move to player character?> */
		/// @brief Calculates the new speed.
		/// @note Applies drag to the current speed to calculate new speed.
		/// If the speed is smaller than the dead zone it is nullified
		/// @param[in] current_speed	The object's current speed.
		/// @param[in] dead_limits		The dead zone.
		/// @return The new speed
		float ApplyDragWithDeadZone(float current_speed, float dead_limits);
	/* </Move to player character?> */

		/// The string name of the object
		std::string tag_;

		/// The current velocity of the object
		gef::Vector4 velocity_;

		/// The model attached to the object
		gef::Model m_model_;

		/// The amount to reduce velocity per frame
		float drag_;

		/// The number of the marker which is currently the parent
		int parent_marker_id_;
	};
}
#endif