#ifndef _ROAD_SEGMENT_H
#define _ROAD_SEGMENT_H

#include "game_object.h"
#include <graphics/renderer_3d.h>
#include <graphics/mesh_instance.h>
#include <list>

namespace hovar
{
	class Platform;

	/**
	The objects which are attached to specific markers.
	*/
	class RoadSegment : public GameObject
	{
	public:
		/// @brief Default constructor.
		/// @note Parent constructor is executed using the platform parameter.
		RoadSegment();

		/// @brief Default destructor.
		/// @note Releases mesh data.
		virtual ~RoadSegment();

		/// @brief Update position.
		/// @note Model and collision volumes are set to the correct world transform.
		void UpdateMeshTransform();

		/// @brief Renders road.
		/// @note If the road's parent marker is visible, render the road.
		/// @param[in] renderer		The 3D rendering engine.
		void Render(gef::Renderer3D * renderer);

		/// @brief Gives road meshes to render.
		/// @param[in] default_mesh		The mesh rendered by default.
		/// @param[in] parent_mesh		The mesh rendered if this is the reference marker.
		void SetMeshes(gef::Mesh* default_mesh, gef::Mesh* parent_mesh) { loose_mesh_ = default_mesh; parent_mesh_ = parent_mesh; }
		
		/// @brief Set the mesh to render.
		/// @param[in] is_parent	Whether to render the reference marker mesh.
		void SetAsParent(bool is_parent);

	private:
		/// The mesh rendered by default.
		gef::Mesh* loose_mesh_;

		/// The mesh rendered if this is the reference marker.
		gef::Mesh* parent_mesh_;
	};
}
#endif

