#ifndef _CUBE_MESH_H
#define _CUBE_MESH_H

// FRAMEWORK FORWARD DECLARATIONS
namespace gef
{
	class Mesh;
	class Platform;
}

namespace hovar
{
	/**
	Builder which creates a cube object
	*/
	class CubeMesh
	{
	public:
		/// @brief Default constructor.
		CubeMesh();
		/// @brief Default destructor.
		~CubeMesh();

		/// @brief Default constructor.
		/// @note Creates cube geometry and collision volumes for aabb and spherical collision.
		/// @param[in] width		The width of the cube.
		/// @param[in] length		The length of the cube.
		/// @param[in] height		The height of the cube.
		/// @param[in] platform		The platform object.
		/// @return					A pointer to the created cube mesh.
		gef::Mesh* CreateCubeMesh(float width, float length, float height, gef::Platform& platform);
	};
}
#endif

