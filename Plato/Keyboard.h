#pragma once
#include "KeyCode.h"

namespace Plato
{
	namespace Input
	{
		/** Interface to check for key and button presses.
		*/
		class Keyboard
		{
		public:
			//! Will return wether or not a key is currently pressed
			static bool GetKey(KEY_CODE key);

			//! Will return wether or not a key has just been pressed down. Only fires once per keypress.
			static bool GetKeyDown(KEY_CODE key);

			//! Will return wether or not a key has just been released. Only fires once per keypress.
			static bool GetKeyUp(KEY_CODE key);
		};
	}
}
