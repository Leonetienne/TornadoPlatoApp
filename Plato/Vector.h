#pragma once

// This file is just an alias for Eule's Vector files.
// It would be very tedios having to include "../Eule/Vector3.h", and then use Eule::Vector every time you want to use vectors
// within the game engine. Much more convenient to just include "Vector.h".
// Also, when using the game engine, one should ideally not have to directly interact with the maths engine at all.

#include "../Eule/Vector2.h"
#include "../Eule/Vector3.h"
#include "../Eule/Vector4.h"

namespace Plato
{
	using Eule::Vector2;
	using Eule::Vector2i;
	using Eule::Vector2d;

	using Eule::Vector3;
	using Eule::Vector3i;
	using Eule::Vector3d;

	using Eule::Vector4;
	using Eule::Vector4i;
	using Eule::Vector4d;
}

