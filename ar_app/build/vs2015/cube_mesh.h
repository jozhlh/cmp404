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
	class CubeMesh
	{
	public:
		CubeMesh();
		~CubeMesh();

		gef::Mesh* CreateCubeMesh(float width, float length, float height, gef::Platform& platform_);
	};
}
#endif

