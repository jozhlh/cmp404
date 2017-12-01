#ifndef _PICKUP_H
#define _PICKUP_H

#include "game_object.h"

namespace hovar
{
	/**
	Collectable items which recharge the player.
	*/
	class Pickup : public GameObject
	{
	public:
		/// @brief Default constructor.
		/// @note initialise energy and collected status.
		Pickup();

		/// @brief Default destructor.
		/// @note No default functionality.
		virtual ~Pickup();

		/// @brief Update function.
		/// @note Gradually roatate collectable.
		/// param[in] dt	Time since last frame (delta time).
		void Update(float dt);

		/// @brief Get energy awarded on pickup.
		/// return The energy to be given to the player.
		float Energy() { return energy_; }

		/// @brief Set whether pickup has been collected.
		/// param[in] dt	Time since last frame (delta time).
		void SetCollected(bool is_collected) { collected_ = is_collected; }

		/// @brief Has the pickup been collected.
		/// return Whether the pickup has been collected.
		bool IsCollected() { return collected_; }

		/// @brief Render the collectable.
		/// param[in] renderer	The 3D rendering engine.
		void Render(gef::Renderer3D* renderer);

	private:
		/// The amount of energy awarded to player on pickup
		float energy_;

		/// Whether the pickup has been collected
		bool collected_;
	};
}

#endif // !_PICKUP_H

