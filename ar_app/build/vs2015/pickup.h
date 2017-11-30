#ifndef _PICKUP_H
#define _PICKUP_H

#include "game_object.h"

namespace hovar
{
	class Pickup : public GameObject
	{
	public:
		Pickup();
		virtual ~Pickup();

		void Update(float dt);
		float Energy() { return energy; }
		void SetCollected(bool isCollected) { collected = isCollected; }
		bool IsCollected() { return collected; }
		void Render(gef::Renderer3D* renderer);

	private:
		float energy;

		bool collected;
	};
}

#endif // !_PICKUP_H

