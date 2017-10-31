#include "camera_manager.h"
#include <maths\math_utils.h>
#include <system/platform.h>

Camera::Camera()
{
	// initialise the camera settings
	eye = gef::Vector4(5.0f, 5.0f, 300.0f);
	lookat = gef::Vector4(0.0f, 0.0f, 0.0f);
	up = gef::Vector4(0.0f, 1.0f, 0.0f);
	camera_fov = gef::DegToRad(45.0f);
	near_plane = 0.01f;
	far_plane = 1000.f;
	view_matrix.SetIdentity();
	projection_matrix.SetIdentity();
}


Camera::~Camera()
{
}

gef::Matrix44 Camera::CalculateViewMatrix()
{
	view_matrix.LookAt(eye, lookat, up);
	return view_matrix;
}

gef::Matrix44 Camera::CalculateProjectionMatrix(gef::Platform& platform)
{
	projection_matrix = platform.PerspectiveProjectionFov(camera_fov, (float)platform.width() / (float)platform.height(), near_plane, far_plane);
	return projection_matrix;
}
