#ifndef _PLAYER_CHARACTER_H
#define _PLAYER_CHARACTER_H

#include "game_object.h"
#include <input\sony_controller_input_manager.h>

namespace hovar
{
	class PlayerCharacter : public GameObject
	{
	private:
		enum State
		{
			alive,
			pickup,
			dead
		};

	public:
		PlayerCharacter();
		virtual ~PlayerCharacter();

		void Update(const gef::SonyController* controller, float dt);
		void UpdateMeshTransform();
		void Render(gef::Renderer3D* renderer);
		void SetRespawnPosition(gef::Matrix44 init_position) { respawn_position_ = init_position; }
		void Respawn();
		void Kill();
		bool IsAlive() { return state_ == alive; }
		bool ParentIsVisible() { return parent_visible_; }
		void GiveEnergy(float energy) { current_energy_ += energy; }
		float GetEnergy() { return current_energy_; }
		void ResetEnergy();

	private:
		void PickupAnim(float dt);
		void DeadAnim(float dt);
		gef::Vector4 Input(const gef::SonyController* controller, float dt);
		
		gef::Matrix44 respawn_position_;
		State state_;
		float dead_counter_;
		float pickup_counter_;
		float dead_duration_;
		float pickup_duration_;
		float current_energy_;
		float energy_decay_rate_;
		float max_speed_;
		float acceleration_;
		float deceleration_;
		float turning_speed_;
	};
}
#endif