#ifndef _ROAD_SEGMENT_H
#define _ROAD_SEGMENT_H

#include "game_object.h"
#include <graphics/renderer_3d.h>
#include <graphics/mesh_instance.h>
#include <list>

namespace hovar
{
	class Platform;

	class RoadSegment : public GameObject
	{
	public:
		RoadSegment();
		virtual ~RoadSegment();
		void UpdateMeshTransform();
		void Render(gef::Renderer3D * renderer);
		void SetMeshes(gef::Mesh* default_mesh, gef::Mesh* parent_mesh) { loose_mesh_ = default_mesh; parent_mesh_ = parent_mesh; }
		void SetAsParent(bool is_parent);

	private:
		gef::Mesh* loose_mesh_;
		gef::Mesh* parent_mesh_;
	};
}
#endif

