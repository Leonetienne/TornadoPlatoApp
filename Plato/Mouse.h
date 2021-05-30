#pragma once
#include "Vector.h"

namespace Plato
{
	namespace Input
	{
		/** Interface to check for mouse-special features.
		*/
		class Mouse
		{
		public:
			//! Will return the global mouse cursor position
			static const Vector2i& GetGlobalMousePosition();

			//! Will return the local mouse cursor position (relative to topleft of this window)
			static const Vector2i& GetLocalMousePosition();

			//! Will return the delta-rotation of the mouse wheel
			static double GetMousewheelDelta();

			//! Will set the global mouse cursor position.  
			//! Returns false if this reverse-event callback is not implemented.
			static bool SetGlobalMousePosition(const Vector2i& newPos);

			//! Will set the local mouse cursor position (relative to topleft of this window).   
			//! Returns false if this reverse-event callback is not implemented.
			static bool SetLocalMousePosition(const Vector2i& newPos);
		};
	}
}
