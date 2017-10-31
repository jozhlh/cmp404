#ifndef _PLAYER_CHARACTER_H
#define _PLAYER_CHARACTER_H

#include "game_object.h"
#include <input\sony_controller_input_manager.h>

class PlayerCharacter : public GameObject
{
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

private:
	gef::Vector4 Input(const gef::SonyController* controller_, float dt);
	
	gef::Matrix44 respawn_position;
	
	float current_energy;
	float energy_decay_ps;
	float max_speed;
	float acceleration;
	float deceleration;
	//float drag;
	float turning_speed;
	
};

#endif