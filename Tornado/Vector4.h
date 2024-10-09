#pragma once

// This file is just an alias for Eule's Vector files.
// It would be very tedios having to include "../Eule/Vector4.h", and then use Eule::Vector every time you want to use vectors
// within the renderer. Much more convenient to just include "Vector4.h".
// Also, when using the game engine, one should ideally not have to directly interact with the maths engine at all.

#include "../Eule/Vector4.h"

namespace TorGL
{
	using Eule::Vector4;
	using Eule::Vector4i;
	using Eule::Vector4d;
}

