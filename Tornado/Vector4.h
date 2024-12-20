#pragma once

// This file is just an alias for Eule's Vector files.
// It would be very tedious having to include "../Eule/Vector4.h", and then use Eule::Vector every time you want to use vectors
// within the renderer. Much more convenient to just include "Vector4.h".
// Also, when using the graphics engine, one should ideally not have to directly interact with the maths engine at all.

#include "../Eule/Vector4.h"

namespace TorGL
{
	using Eule::Vector4;
	using Eule::Vector4i;
	using Eule::Vector4d;
}

extern template const Eule::Vector4<double> Eule::Vector4<double>::left;
extern template const Eule::Vector4<double> Eule::Vector4<double>::right;
extern template const Eule::Vector4<double> Eule::Vector4<double>::up;
extern template const Eule::Vector4<double> Eule::Vector4<double>::down;
extern template const Eule::Vector4<double> Eule::Vector4<double>::forward;
extern template const Eule::Vector4<double> Eule::Vector4<double>::backward;
extern template const Eule::Vector4<double> Eule::Vector4<double>::future;
extern template const Eule::Vector4<double> Eule::Vector4<double>::past;
extern template const Eule::Vector4<double> Eule::Vector4<double>::one;
extern template const Eule::Vector4<double> Eule::Vector4<double>::zero;
