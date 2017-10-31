#include "camera_manager.h"
#include <maths\math_utils.h>
#include <system/platform.h>

Camera::Camera()
{
	// initialise the camera settings
	eye_ = gef::Vector4(5.0f, 5.0f, 300.0f);
	lookat_ = gef::Vector4(0.0f, 0.0f, 0.0f);
	up_ = gef::Vector4(0.0f, 1.0f, 0.0f);
	camera_fov_ = gef::DegToRad(45.0f);
	near_plane_ = 0.01f;
	far_plane_ = 1000.f;
	view_matrix_.SetIdentity();
	projection_matrix_.SetIdentity();
}


Camera::~Camera()
{
}

gef::Matrix44 Camera::CalculateViewMatrix()
{
	view_matrix_.LookAt(eye_, lookat_, up_);
	return view_matrix_;
}

gef::Matrix44 Camera::CalculateProjectionMatrix(gef::Platform& platform)
{
	projection_matrix_ = platform.PerspectiveProjectionFov(camera_fov_, (float)platform.width() / (float)platform.height(), near_plane_, far_plane_);
	return projection_matrix_;
}
