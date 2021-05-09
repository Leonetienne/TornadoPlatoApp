#pragma once

namespace Input
{
	/** Describes the state of a key
	*/
	enum class KEY_STATE
	{
		NONE,
		HELD,
		RELEASED,
		DOWN,
		UP
	};
}
