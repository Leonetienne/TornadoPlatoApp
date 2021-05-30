#pragma once

// This file is just an alias for Tornado's material file.
// It would be very tedios having to include "../Tornado/Material.h", and then use TorGL::Material every time you want to use materials
// within the game engine. Much more convenient to just include "Material.h".
// Also, when using the game engine, one should ideally not have to directly interact with the graphics engine at all.

#include "../Tornado/Material.h"

namespace Plato
{
	using TorGL::Material;
}
