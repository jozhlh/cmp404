#pragma once

#include <maths/vector4.h>
#include <maths/matrix44.h>

// FRAMEWORK FORWARD DECLARATIONS
namespace gef
{
	class Platform;
}

namespace hovar
{
	/**
	Stores and calculates camera variables
	*/
	class Camera
	{

	public:
		/// @brief Default constructor.
		/// @note Initialises all member variables to starting values.
		Camera();

		/// @brief Default destructor.
		~Camera();

		/// @brief Sets camera position.
		/// @note View matrix is calculated according to new camera position.
		/// @param[in] pos		The camera's new position.
		void SetEyePosition(gef::Vector4 pos) { eye_ = pos; view_matrix_ = CalculateViewMatrix(); }
		
		/// @brief Sets camera direction.
		/// @note View matrix is calculated according to the direction the camera is facing.
		/// @param[in] target	The point that the camera is now facing.
		void SetLookAt(gef::Vector4 target) { lookat_ = target; view_matrix_ = CalculateViewMatrix(); }
		
		/// @brief Sets camera orientation.
		/// @note View matrix is calculated according to how the camera is oriented.
		/// @param[in] new_up	The new up direction for the camera.
		void SetUp(gef::Vector4 new_up) { up_ = new_up; CalculateViewMatrix(); }

		/// @brief Returns camera position.
		/// @return		The camera's position.
		gef::Vector4 Eye() { return eye_; }

		/// @brief Returns camera's direction.
		/// @return		The point the camera is facing.
		gef::Vector4 LookAt() { return lookat_; }

		/// @brief Returns camera's orientation.
		/// @return		The direction which is up for the camera.
		gef::Vector4 Up() { return up_; }

		/// @brief Calculates the view matrix.
		/// @note Uses the current position, direction and orientation to construct a view matrix.
		/// @return		The view matrix.
		gef::Matrix44 CalculateViewMatrix();

		/// @brief Calculates the projection matrix.
		/// @note Uses the platform's screen size with the camera's data to calculate a projection matrix.
		/// @param[in] platform		The platform object.
		/// @return		The projection matrix.
		gef::Matrix44 CalculateProjectionMatrix(gef::Platform& platform);

	private:
		/// The current view matrix.
		gef::Matrix44 view_matrix_;

		/// The current projection matrix.
		gef::Matrix44 projection_matrix_;

		/// The camera's position.
		gef::Vector4 eye_;

		/// The direction the camera is facing.
		gef::Vector4 lookat_;

		/// The orientation of the camera.
		gef::Vector4 up_;

		/// The camera's field of view.
		float camera_fov_;

		/// The closest distance to render.
		float near_plane_;

		/// The furthest distance to render.
		float far_plane_;
	};
}
