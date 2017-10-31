#ifndef _PLAYER_CHARACTER_H
#define _PLAYER_CHARACTER_H

#include <game_object.h>
#include <input\sony_controller_input_manager.h>

class PlayerCharacter : public GameObject
{
public:
	PlayerCharacter();
	~PlayerCharacter();
	
	void Input(const gef::SonyController* controller_);

private:
	float movement_speed;

};

#endif