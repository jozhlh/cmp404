#pragma once

#include <maths/vector4.h>
#include <maths/matrix44.h>

// FRAMEWORK FORWARD DECLARATIONS
namespace gef
{
	class Platform;
}

class Camera
{

public:
	Camera();
	~Camera();
	void SetEyePosition(gef::Vector4 pos) { eye = pos; view_matrix = CalculateViewMatrix(); }
	void SetLookAt(gef::Vector4 target) { lookat = target; view_matrix = CalculateViewMatrix(); }
	void SetUp(gef::Vector4 newUp) { up = newUp; CalculateViewMatrix(); }
	gef::Vector4 Eye() { return eye; }
	gef::Vector4 LookAt() { return lookat; }
	gef::Vector4 Up() { return up; }
	gef::Matrix44 CalculateViewMatrix();
	gef::Matrix44 CalculateProjectionMatrix(gef::Platform& platform);

private:
	gef::Matrix44 view_matrix;
	gef::Matrix44 projection_matrix;
	gef::Vector4 eye;
	gef::Vector4 lookat;
	gef::Vector4 up;
	float camera_fov;
	float near_plane;
	float far_plane;
};

