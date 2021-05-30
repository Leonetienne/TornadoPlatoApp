#pragma once

// This file is just an alias for Tornado's texture file.
// It would be very tedios having to include "../Tornado/Texture.h", and then use TorGL::Texture every time you want to use textures
// within the game engine. Much more convenient to just include "Texture.h".
// Also, when using the game engine, one should ideally not have to directly interact with the graphics engine at all.

#include "../Tornado/Texture.h"

namespace Plato
{
	using TorGL::Texture;
}
