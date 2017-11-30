#include "road_segment.h"

namespace hovar
{
	RoadSegment::RoadSegment()
	{
		GameObject();
	}


	RoadSegment::~RoadSegment()
	{
		delete loose_mesh_;
		loose_mesh_ = NULL;

		delete parent_mesh_;
		parent_mesh_ = NULL;
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
