#ifndef MAIN_LEVEL_H_
#define MAIN_LEVEL_H_

#include "base_scene.h"

#include "assets\obj_loader.h"
#include "graphics\font.h"
#include "graphics\sprite.h"
#include "platform\vita\graphics\texture_vita.h"

#include "game_object_manager.h"
#include "pickup_manager.h"
#include "road_segment.h"
#include "cube_mesh.h"

namespace hovar
{
	class MainLevel : public BaseScene
	{
	public:
		MainLevel();
		~MainLevel();

		void Init(gef::Platform* platform);

	private:
		void InitFont();

		gef::InputManager* input_manager_;
		gef::Renderer3D* renderer_3d_;
		gef::SpriteRenderer* sprite_renderer_;
		gef::Font* font_;
		gef::Sprite* camera_display_;
		gef::TextureVita* camera_texture_;
		gef::Matrix44 orthographic_projection_;
		gef::OBJLoader obj_loader_;
		gef::Matrix44 perspective_projection_fov_;

		GameObjectManager* game_object_manager_;
		PickupManager* pickup_manager_;
		PlayerCharacter* player_character_;
		RoadSegment* road_cross_;
		CubeMesh cube_builder_;

		float kYScalingFactor_;
		float kGridSize;
	};
}


#endif

