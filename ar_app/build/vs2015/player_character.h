#ifndef _PLAYER_CHARACTER_H
#define _PLAYER_CHARACTER_H

#include "game_object.h"
#include <input\sony_controller_input_manager.h>

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
	
	void Update(const gef::SonyController* controller_, float dt);
	void UpdateMeshTransform();
	void Render(gef::Renderer3D* renderer);
	void Rebound(gef::Vector4 collision_normal);
	void SetRespawnPosition(gef::Matrix44 init_position) { respawn_position = init_position; }
	void Respawn();
	void GiveEnergy(float energy) { current_energy += energy; }
	float Energy() { return current_energy; }
	void ResetEnergy() { current_energy = 20.0f; pickup_counter_ = 0.0f; state_ = pickup; }
	void Kill();
	bool IsAlive() { return state_ == alive; }

private:
	void PickupAnim(float dt);
	void DeadAnim(float dt);
	gef::Vector4 Input(const gef::SonyController* controller_, float dt);
	gef::Matrix44 respawn_position;
	
	State state_;
	float dead_counter_;
	float pickup_counter_;
	float dead_duration_;
	float pickup_duration_;
	float current_energy;
	float energy_decay_ps;
	float max_speed;
	float acceleration;
	float deceleration;
	//float drag;
	float turning_speed;
	
};

#endif