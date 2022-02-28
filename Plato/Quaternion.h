#pragma once

// This file is just an alias for Eule's quaternion file.
// It would be very tedios having to include "../Eule/Quaternion.h", and then use Eule::Quaternion every time you want to use quaternions
// within the game engine. Much more convenient to just include "Quaternion.h".
// Also, when using the game engine, one should ideally not have to directly interact with the maths engine at all.

#include <Eule/Eule/Quaternion.h>

namespace Plato
{
	using Eule::Quaternion;
}
