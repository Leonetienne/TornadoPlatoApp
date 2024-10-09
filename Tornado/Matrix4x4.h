#pragma once

// This file is just an alias for Eule's Matrix4x4 file.
// It would be very tedios having to include "../Eule/Matrix4x4.h", and then use Eule::Matrix4x4 every time you want to use matrices
// within the renderer. Much more convenient to just include "Matrix4x4.h".
// Also, when using the game engine, one should ideally not have to directly interact with the maths engine at all.

#include "../Eule/Matrix4x4.h"

namespace TorGL
{
	using Eule::Matrix4x4;
}
