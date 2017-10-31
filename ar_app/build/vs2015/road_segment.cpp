#include "road_segment.h"


RoadSegment::RoadSegment()
{
	GameObject();
	walls = new bool[9];
	for (int x = 0; x < 3; x++)
	{
		for (int z = 0; z < 3; z++)
		{
			walls[(x * 3) + z] = false;
		}
	}
}


RoadSegment::~RoadSegment()
{
	for (int x = 0; x < 3; x++)
	{
		for (int z = 0; z < 3; z++)
		{
			delete wall_cubes[x][z];
			wall_cubes[x][z] = NULL;
		}
	}
}

void RoadSegment::InitWallCollisionBoxes(RoadType shape, gef::Mesh * wall_mesh, float grid_size, gef::Matrix44 root_transform)
{
	walls = RoadConfigs::GetConfigs(shape, walls);
	gef::Matrix44 offset = root_transform;
	for (int x = 0; x < 3; x++)
	{
		for (int z = 0; z < 3; z++)
		{
			if (walls[(x * 3) + z])
			{
				wall_offsets[x][z] = root_transform;
				wall_offsets[x][z].SetTranslation(
					gef::Vector4((-1 * grid_size) + (x * grid_size), (-1 * grid_size) + (z * grid_size), 0.0f));
				wall_cubes[x][z] = new gef::MeshInstance();
				wall_cubes[x][z]->set_mesh(wall_mesh);
				wall_cubes[x][z]->set_transform(wall_offsets[x][z]);
			}
			else
			{
				wall_cubes[x][z] = NULL;
			}
		}
	}
}

void RoadSegment::UpdateMeshTransform()
{
	transform_ = m_mv_transform_.GetMatrix() * m_transform_->GetMatrix();
	collider_->set_transform(collider_offset * m_transform_->GetMatrix());
	for (int x = 0; x < 3; x++)
	{
		for (int z = 0; z < 3; z++)
		{
			if (walls[(x * 3) + z])
			{
				wall_cubes[x][z]->set_transform(wall_offsets[x][z] * m_transform_->GetMatrix());
			}
		}
	}
}

void RoadSegment::Render(gef::Renderer3D * renderer)
{
	if (parent_visible_)
	{
		renderer->DrawMesh(*this);
		//renderer->DrawMesh(*collider_);
		/*
		for (int x = 0; x < 3; x++)
		{
			for (int z = 0; z < 3; z++)
			{
				if (walls[(x * 3) + z])
				{
					renderer->DrawMesh(*wall_cubes[x][z]);
				}
			}
		}
		*/
	}
}

std::list<gef::MeshInstance*> RoadSegment::GetWallCubes()
{
	std::list<gef::MeshInstance*> active_walls;
	for (int x = 0; x < 3; x++)
	{
		for (int z = 0; z < 3; z++)
		{
			if (walls[(x * 3) + z])
			{
				active_walls.push_back(wall_cubes[x][z]);
			}
		}
	}
	return active_walls;
}
