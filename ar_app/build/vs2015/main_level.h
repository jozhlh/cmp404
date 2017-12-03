/*
	main_level.h

	Main level which runs the game.

	@author	Josh Hale
	Last Edited: 02/12/17
*/

#ifndef _MAIN_LEVEL_H
#define _MAIN_LEVEL_H

#include "base_scene.h"

#include <sony_sample_framework.h>

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
	/**
	Main level which runs the game.
	*/
	class MainLevel : public BaseScene
	{
	private: 
		/// States that the main game loop can be in
		/// Game is either running or over
		enum State
		{
			running,
			finished
		};

	public:
		/// @brief Default constructor.
		/// @note Initialises framerate and score.
		MainLevel();

		/// @brief Default destructor.
		/// @note No default functionality.
		~MainLevel();

		/// @brief Initialises main level.
		/// @note Creates and initialises all objects in the scene.
		/// @param[in] platform		The platform object.
		void Init(gef::Platform* platform);

		/// @brief Main gameplay loop.
		/// @note Performs all gameplay logic.
		/// @param[in] frame_time	Time since the last update function (delta time).
		/// @return False if player wishes to quit to main menu.
		bool Update(const float frame_time);

		/// @brief Renders scene.
		/// @note Renders all 3D objects and UI. Only renders Game Over screen if game state is finished.
		void Render();

		/// @brief Releases unused memory.
		void CleanUp();

	private:
		/// @brief Resets level.
		void Restart();

		/// @brief Loads font data.
		/// @note Loads the comic sans font.
		/// http://explosm.net/comics/2301/
		void InitFont();

		/// @brief Creates lights.
		/// @note Creates a point light and sets the ambient light.
		void InitLights();

		/// @brief Initialise camera.
		/// @note Initialise vita rear camera display variables.
		void InitCameraDisplay();

		/// @brief Initialise road objects.
		/// @note Initialise the objects which are bound to specific markers.
		/// @param[in] mv_scale		The art asset scaling factor.
		void InitRoads(float mv_scale);

		/// @brief Initialise player character.
		/// @param[in] mv_scale		The art asset scaling factor.
		void InitPlayerCharacter(float mv_scale);

		/// @brief Initialise the pickup manager.
		/// @param[in] mv_scale		The art asset scaling factor.
		void InitPickups(float mv_scale);

		/// @brief Executes Collision Detection.
		/// @note Tests for collisions between the player and roads / pickups.
		void CheckForCollisions();

		/// @brief Updates marker data.
		/// @note Looks for AR markers and updates stored data.
		void UpdateMarkers();

		/// @brief The core game loop.
		/// @note Runs the logic which occurs whilst the game is running.
		/// @param[in] dt	Time since the last update function (delta time).
		void GameLogic(float dt);

		/// @brief The game over loop.
		/// @note Runs the logic which occurs whilst the game is over.
		/// @return Returns false if the user has selected to return to main menu.
		bool GameOverLogic();

		/// @brief Render camera feed.
		/// @param[in] dat	Structure containing camera feed image
		void RenderCameraFeed(struct ::AppData* dat);

		/// @brief Render all objects.
		void Render3DScene();

		/// @brief Render UI.
		void RenderOverlay();

		/// @brief Draw UI components.
		/// @note Renders the framerate counter as well as the player's score and energy.
		void DrawHUD();

		/// @brief Render game over UI.
		void RenderGameOver();

		/// The 3D rendering engine.
		gef::Renderer3D* renderer_3d_;

		/// The 2D rendering engine.
		gef::SpriteRenderer* sprite_renderer_;

		/// The font data.
		gef::Font* font_;

		/// The sprite which holds the camera feed data.
		gef::Sprite* camera_display_;

		/// The texture which holds the camera feed data to be given to sprite.
		gef::TextureVita* camera_texture_;

		/// Orthograohic projection matrix for 2D rendering.
		gef::Matrix44 orthographic_projection_;
		
		/// Perspective projection matrix for 3D rendering.
		gef::Matrix44 perspective_projection_fov_;

		/// The obj loader for loading 3D art assets and textures.
		gef::OBJLoader obj_loader_;

		/// Whether the game is currently running or finished.
		State game_state_;

		/// The manager which handles all game objects.
		GameObjectManager* game_object_manager_;

		/// The manager which handles all collectables.
		PickupManager* pickup_manager_;

		/// The player controlled vehicle.
		PlayerCharacter* player_character_;

		/// The road objects which are bound to specific markers.
		RoadSegment* road_[NUM_OF_MARKERS];

		/// The default road mesh data.
		gef::Mesh* road_mesh_;

		/// The reference marker mesh data.
		gef::Mesh* parent_mesh_;

		/// The default road model.
		gef::Model* road_model_;

		/// The reference road model.
		gef::Model* parent_model_;

		/// The builder of cubes.
		CubeMesh cube_builder_;

		/// The player's current score.
		float score_;
	};
}

#endif //_MAIN_LEVEL_H

