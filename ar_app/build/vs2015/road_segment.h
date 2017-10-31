#ifndef _ROAD_SEGMENT_H
#define _ROAD_SEGMENT_H

#include "game_object.h"
#include <graphics/renderer_3d.h>
#include <graphics/mesh_instance.h>
#include "road_configs.h"
#include <list>

class Platform;

class RoadSegment : public GameObject
{
public:
	RoadSegment();
	virtual ~RoadSegment();
	void InitWallCollisionBoxes(RoadType shape, gef::Mesh* wall_mesh, float grid_size, gef::Matrix44 root_transform);

	void UpdateMeshTransform();
	void Render(gef::Renderer3D * renderer);
	std::list<gef::MeshInstance*> GetWallCubes();

private:
	gef::MeshInstance* wall_cubes[3][3];
	gef::Matrix44 wall_offsets[3][3];
	bool* walls;
};

#endif

