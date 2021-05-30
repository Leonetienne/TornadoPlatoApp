#pragma once

namespace Plato
{
	namespace Input
	{
		/** Types of reverse-event callbacks.
		* Use these with EventHandler::RegisterReverseEventCallback() and an std::functional object to provide these functionalities
		*/
		enum class REVERSE_EVENT_CALLBACK
		{
			//! For exiting the application
			EXIT,

			//! For setting the global mouse position. Comes with one argument: Vector2i
			SET_GLOBAL_MOUSE_POSITION,

			//! For setting the mouse position relative to the window. Comes with one argument: Vector2i
			SET_LOCAL_MOUSE_POSITION
		};
	}
}
