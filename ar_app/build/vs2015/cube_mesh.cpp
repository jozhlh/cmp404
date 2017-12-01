#include "cube_mesh.h"
#include <system\platform.h>
#include <graphics\mesh.h>
#include <graphics\primitive.h>

namespace hovar
{
	CubeMesh::CubeMesh()
	{
	}


	CubeMesh::~CubeMesh()
	{
	}

	gef::Mesh * CubeMesh::CreateCubeMesh(float width, float length, float height, gef::Platform& platform)
	{
		gef::Mesh* mesh = platform.CreateMesh();

		// initialise the vertex data to create a width * length * height cube
		const float half_x = width * 0.5f;
		const float half_y = height * 0.5f;
		const float half_z = length * 0.5f;

		const gef::Mesh::Vertex vertices[] = {
			{ half_x, -half_y, -half_z,  0.577f, -0.577f, -0.577f, 0.0f, 0.0f },
			{ half_x,  half_y, -half_z,  0.577f,  0.577f, -0.577f, 0.0f, 0.0f },
			{ -half_x,  half_y, -half_z, -0.577f,  0.577f, -0.577f, 0.0f, 0.0f },
			{ -half_x, -half_y, -half_z, -0.577f, -0.577f, -0.577f, 0.0f, 0.0f },
			{ half_x, -half_y,  half_z,  0.577f, -0.577f,  0.577f, 0.0f, 0.0f },
			{ half_x,  half_y,  half_z,  0.577f,  0.577f,  0.577f, 0.0f, 0.0f },
			{ -half_x,  half_y,  half_z, -0.577f,  0.577f,  0.577f, 0.0f, 0.0f },
			{ -half_x, -half_y,  half_z, -0.577f, -0.577f,  0.577f, 0.0f, 0.0f }
		};

		mesh->InitVertexBuffer(platform, static_cast<const void*>(vertices), sizeof(vertices) / sizeof(gef::Mesh::Vertex), sizeof(gef::Mesh::Vertex));

		// create a single triangle list primitive to draw the triangles that make up the cube
		mesh->AllocatePrimitives(1);
		gef::Primitive* primitive = mesh->GetPrimitive(0);

		const UInt32 indices[] = {
			// Back
			0, 1, 2,
			2, 3, 0,
			// Front
			6, 5, 4,
			4, 7, 6,
			// Left
			2, 7, 3,
			2, 6, 7,
			// Right
			0, 4, 1,
			5, 1, 4,
			// Top
			6, 2, 1,
			5, 6, 1,
			// Bottom
			0, 3, 7,
			0, 7, 4
		};

		primitive->InitIndexBuffer(platform_, static_cast<const void*>(indices), sizeof(indices) / sizeof(UInt32), sizeof(UInt32));
		primitive->set_type(gef::TRIANGLE_LIST);

		// set size of bounds, we need this for collision detection routines
		gef::Aabb aabb(gef::Vector4(-half_x, -half_y, -half_z), gef::Vector4(half_x, half_y, half_z));
		gef::Sphere sphere(aabb);
		gef::Sphere small_sphere(gef::Vector4(0.0f, 0.0f, 0.0f), half_x);

		mesh->set_aabb(aabb);
		mesh->set_bounding_sphere(small_sphere);

		return mesh;
	}
}