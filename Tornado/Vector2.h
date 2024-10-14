#pragma once

// This file is just an alias for Eule's Vector files.
// It would be very tedious having to include "../Eule/Vector2.h", and then use Eule::Vector every time you want to use vectors
// within the renderer. Much more convenient to just include "Vector2.h".
// Also, when using the grahics engine, one should ideally not have to directly interact with the maths engine at all.

#include "../Eule/Vector2.h"

namespace TorGL
{
	using Eule::Vector2;
	using Eule::Vector2i;
	using Eule::Vector2d;
}

