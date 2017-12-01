#ifndef _BASE_SCENE_H_
#define _BASE_SCENE_H_

#include "input\input_manager.h"

namespace hovar
{
	/**
	All default scene functionality is contained in the base scene class.
	*/
	class BaseScene
	{
	public:
		/// @brief Default constructor.
		/// @note The framerate variable is initialised.
		BaseScene();

		/// @brief Virtual destructor.
		/// @note The CleanUp function is executed to release unused memory.
		virtual ~BaseScene();

		/// @brief Virtual initialiser.
		/// @note Obtain reference to the platform object and create an input manager.
		/// @param[in] platform		The platform object.
		virtual void Init(gef::Platform *platform);

		/// @brief Releases unused memory.
		virtual void CleanUp();

		/// @brief Virtual update.
		/// @note Calculates framerate and gets new controller input.
		/// @param[in] frame_time		The time since the last frame (delta time).
		/// @return Always returns true.
		virtual bool Update(const float frame_time);7

		/// @brief Virtual render.
		/// @note No functionality, purely for interface.
		virtual void Render();


	protected:
		/// Reference to the platform object.
		gef::Platform* platform_;

		/// Object which receives input from the device
		gef::InputManager* input_manager_;

		/// The current framerate
		float fps_;
	};
}


#endif //_BASE_SCENE_H_

