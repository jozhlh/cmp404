/*
	road_segment.cpp

	The objects which are attached to specific markers.

	@author	Josh Hale
	Last Edited: 02/12/17
*/

#include "road_segment.h"

namespace hovar
{
	RoadSegment::RoadSegment()
	{
	}


	RoadSegment::~RoadSegment()
	{
	}

	void RoadSegment::UpdateMeshTransform()
	{
		transform_ = m_mv_transform_.GetMatrix() * m_transform_->GetMatrix();
		collider_->set_transform(collider_offset_ * m_transform_->GetMatrix());
		obb_->SetCoordinateFrameFromMatrix(collider_offset_ * m_transform_->GetMatrix(), "road");
	}

	void RoadSegment::Render(gef::Renderer3D * renderer)
	{
		if (parent_visible_)
		{
			renderer->DrawMesh(*this);
			//renderer->DrawMesh(*collider_);
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
}
