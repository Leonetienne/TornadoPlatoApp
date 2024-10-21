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

extern template const Eule::Vector2<double> Eule::Vector2<double>::left;
extern template const Eule::Vector2<double> Eule::Vector2<double>::right;
extern template const Eule::Vector2<double> Eule::Vector2<double>::up;
extern template const Eule::Vector2<double> Eule::Vector2<double>::down;
extern template const Eule::Vector2<double> Eule::Vector2<double>::one;
extern template const Eule::Vector2<double> Eule::Vector2<double>::zero;
