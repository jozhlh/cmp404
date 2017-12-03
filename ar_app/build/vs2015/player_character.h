/*
	player_character.h

	The player controlled vehicle.

	@author	Josh Hale
	Last Edited: 02/12/17
*/

#ifndef _PLAYER_CHARACTER_H
#define _PLAYER_CHARACTER_H

#include "game_object.h"
#include <input\sony_controller_input_manager.h>

namespace hovar
{
	/**
	The player controlled vehicle.
	*/
	class PlayerCharacter : public GameObject
	{
	private:
		/// States that the player can be in.
		/// alive	The player's default state.
		/// pickup	The player just collected a pickup.
		/// dead	The player just left a road.
		enum State
		{
			alive,
			pickup,
			dead
		};

	public:
		using GameObject::Update;
		/// @brief Default constructor.
		/// @note Player movement variables are initialised.
		PlayerCharacter();

		/// @brief Default destructor.
		/// @note Collider is released to free up unused memory.
		~PlayerCharacter();

		/// @brief Update executes every frame.
		/// @note Test for input to change movement and move vehicle according to its velocity.
		/// @param[in] controller	The controller to read for input.
		/// @param[in] dt			Time since the last update function (delta time).
		void Update(const gef::SonyController* controller, float dt);

		/// @brief Update position.
		/// @note Model and collision volumes are set to the correct world transform.
		void UpdateMeshTransform();

		/// @brief Renders player.
		/// @note If the player's parent marker is visible, render the player.
		/// @param[in] renderer		The 3D rendering engine.
		void Render(gef::Renderer3D* renderer);

		/// @brief Sets the root position.
		/// @note Sets the local position the player is respawned to on their parent marker object.
		/// @param[in] init_position		The local position the player spawns at.
		void SetRespawnPosition(gef::Matrix44 init_position) { respawn_position_ = init_position; }

		/// @brief Reset player position.
		/// @note Nullifies player's velocity and moves them to the respawn position.
		void Respawn();

		/// @brief Starts the death sequence.
		/// @note Changes player's state to dead and begins death sequence.
		void Kill();

		/// @brief Checks if player is alive.
		/// @return Whether the player is alive.
		bool IsAlive() { return state_ == alive; }

		/// @brief Checks if parent marker is being tracked.
		/// @return Whether the player's parent marker is visible by the camera.
		bool ParentIsVisible() { return parent_visible_; }

		/// @brief Increase energy level.
		/// @param[in] energy	Amount to increase player's energy level by.
		void GiveEnergy(float energy) { current_energy_ += energy; }

		/// @brief Get player's energy level.
		/// @return How much energy the player currently has.
		float GetEnergy() { return current_energy_; }

		/// @brief Set energy level to default.
		void ResetEnergy();

	private:
		/// @brief Pickup collected sequence.
		/// @note Executes while the player is in pickup state, visual feedback that pickup has been collected.
		/// @param[in] dt	Time since the last update function (delta time).
		void PickupAnim(float dt);

		/// @brief Death sequence.
		/// @note Executes while the player is in death state, visual feedback that player has fallen off road.
		/// @param[in] dt	Time since the last update function (delta time).
		void DeadAnim(float dt);

		/// @brief Gets input from controller.
		/// @note Checks face buttons for acceleration and applies this to forward vector.
		/// Also checks left analogue stick for information on how much to rotate the player.
		/// @param[in] controller	The controller to read for input.
		/// @param[in] dt			Time since the last update function (delta time).
		gef::Vector4 Input(const gef::SonyController* controller, float dt);
		
		/// The local position the player is reset to after death.
		gef::Matrix44 respawn_position_;

		/// The current state of the player.
		State state_;

		/// How long since the player died.
		float dead_counter_;

		/// How long since the player collected a pickup.
		float pickup_counter_;

		/// How long the death sequence lasts.
		float dead_duration_;

		/// How long the pickup collected sequence lasts.
		float pickup_duration_;

		/// The current energy level of the player.
		float current_energy_;

		/// How much the enrgy level decreases per second.
		float energy_decay_rate_;

		/// The fastest the player can move in any one direction.
		float max_speed_;

		/// How quickly the accelerator moves the player forward.
		float acceleration_;

		/// How quickly the decelerator stops and moves the player backward.
		float deceleration_;

		/// How quickly the player rotates.
		float turning_speed_;
	};
}
#endif //_PLAYER_CHARACTER_H