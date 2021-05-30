#pragma once

namespace Plato
{
	namespace Input
	{
		/** Describes the state of a key
		*/
		enum class KEY_STATE
		{
			NONE,
			HELD,		// Currently held down
			RELEASED,	// Currently not pressed
			DOWN,		// First frame of being pressed down
			UP			// First frame of being released
		};
	}
}
