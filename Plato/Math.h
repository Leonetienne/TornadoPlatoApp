#pragma once

// This file is just an alias for Eule's math file.
// It would be very tedios having to include "../Eule/Math.h", and then use Eule::Math every time you want to use math methods
// within the game engine. Much more convenient to just include "Math.h".
// Also, when using the game engine, one should ideally not have to directly interact with the maths engine at all.

#include "Eule/Math.h"

namespace Plato
{
	using Eule::Math;
}
