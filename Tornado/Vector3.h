#pragma once

// This file is just an alias for Eule's Vector files.
// It would be very tedios having to include "../Eule/Vector3.h", and then use Eule::Vector every time you want to use vectors
// within the renderer. Much more convenient to just include "Vector3.h".
// Also, when using the game engine, one should ideally not have to directly interact with the maths engine at all.

#include "../Eule/Vector3.h"

namespace TorGL
{
	using Eule::Vector3;
	using Eule::Vector3i;
	using Eule::Vector3d;
}

