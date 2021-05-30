#pragma once

// This file is just an alias for Tornado's color file.
// It would be very tedios having to include "../Tornado/Color.h", and then use TorGL::Color every time you want to use colors
// within the game engine. Much more convenient to just include "Color.h".
// Also, when using the game engine, one should ideally not have to directly interact with the graphics engine at all.

#include "../Tornado/Color.h"

namespace Plato
{
	using TorGL::Color;
}
