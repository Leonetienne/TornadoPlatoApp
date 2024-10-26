#pragma once

// This file is just an alias for Eule's Vector files.
// It would be very tedios having to include "../Eule/Vector3.h", and then use Eule::Vector every time you want to use vectors
// within the game engine. Much more convenient to just include "Vector.h".
// Also, when using the game engine, one should ideally not have to directly interact with the maths engine at all.

#include "../Tornado/Vector2.h"
#include "../Tornado/Vector3.h"
#include "../Tornado/Vector4.h"

namespace Plato
{
	using TorGL::Vector2;
	using TorGL::Vector2i;
	using TorGL::Vector2d;

	using TorGL::Vector3;
	using TorGL::Vector3i;
	using TorGL::Vector3d;

	using TorGL::Vector4;
	using TorGL::Vector4i;
	using TorGL::Vector4d;
}
