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

			delete wall_obbs[x][z];
			wall_obbs[x][z] = NULL;
		}
	}
}

void RoadSegment::InitWallCollisionBoxes(RoadType shape, gef::Mesh * wall_mesh, float grid_size, gef::Matrix44 root_transform, gef::Vector4 collider_size)
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


				wall_obbs[x][z] = new obb::OBB(obb::Vector(0.5f * collider_size.x(), 0.5f * collider_size.z(), 0.5f * collider_size.y()));
				wall_obbs[x][z]->SetCoordinateFrameFromMatrix(wall_offsets[x][z] * root_transform, "road");
			}
			else
			{
				wall_cubes[x][z] = NULL;
				wall_obbs[x][z] = NULL;
			}
		}
	}
}


void RoadSegment::UpdateMeshTransform()
{
	transform_ = m_mv_transform_.GetMatrix() * m_transform_->GetMatrix();
	collider_->set_transform(collider_offset * m_transform_->GetMatrix());
	obb_->SetCoordinateFrameFromMatrix(collider_offset * m_transform_->GetMatrix(), "road");
	for (int x = 0; x < 3; x++)
	{
		for (int z = 0; z < 3; z++)
		{
			if (walls[(x * 3) + z])
			{
				wall_cubes[x][z]->set_transform(wall_offsets[x][z] * m_transform_->GetMatrix());
				wall_obbs[x][z]->SetCoordinateFrameFromMatrix(wall_offsets[x][z] * m_transform_->GetMatrix(), "wall");
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

void RoadSegment::SetAsParent(bool is_parent)
{
	if (is_parent)
	{
		set_mesh(parent_mesh_);
	}
	else
	{
		set_mesh(loose_mesh_);
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

std::list<obb::OBB*> RoadSegment::GetWallObbs()
{
	std::list<obb::OBB*> active_walls;
	for (int x = 0; x < 3; x++)
	{
		for (int z = 0; z < 3; z++)
		{
			if (walls[(x * 3) + z])
			{
				active_walls.push_back(wall_obbs[x][z]);
			}
		}
	}
	return active_walls;
}
