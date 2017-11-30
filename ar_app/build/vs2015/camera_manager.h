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
	class Camera
	{

	public:
		Camera();
		~Camera();
		void SetEyePosition(gef::Vector4 pos) { eye_ = pos; view_matrix_ = CalculateViewMatrix(); }
		void SetLookAt(gef::Vector4 target) { lookat_ = target; view_matrix_ = CalculateViewMatrix(); }
		void SetUp(gef::Vector4 newUp) { up_ = newUp; CalculateViewMatrix(); }
		gef::Vector4 Eye() { return eye_; }
		gef::Vector4 LookAt() { return lookat_; }
		gef::Vector4 Up() { return up_; }
		gef::Matrix44 CalculateViewMatrix();
		gef::Matrix44 CalculateProjectionMatrix(gef::Platform& platform);

	private:
		gef::Matrix44 view_matrix_;
		gef::Matrix44 projection_matrix_;
		gef::Vector4 eye_;
		gef::Vector4 lookat_;
		gef::Vector4 up_;
		float camera_fov_;
		float near_plane_;
		float far_plane_;
	};
}
