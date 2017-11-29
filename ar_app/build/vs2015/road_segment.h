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
	void InitWallCollisionBoxes(RoadType shape, gef::Mesh* wall_mesh, float grid_size, gef::Matrix44 root_transform, gef::Vector4 collider_size);
	void UpdateMeshTransform();
	void Render(gef::Renderer3D * renderer);
	void SetMeshes(gef::Mesh* default_mesh, gef::Mesh* parent_mesh) { loose_mesh_ = default_mesh; parent_mesh_ = parent_mesh; }
	void SetAsParent(bool is_parent);
	std::list<gef::MeshInstance*> GetWallCubes();
	std::list<obb::OBB*> GetWallObbs();

private:
	gef::Mesh* loose_mesh_;
	gef::Mesh* parent_mesh_;
	gef::MeshInstance* wall_cubes[3][3];
	obb::OBB* wall_obbs[3][3];
	gef::Matrix44 wall_offsets[3][3];
	bool* walls;
};

#endif

