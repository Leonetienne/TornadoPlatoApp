#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "Color.h"

namespace TorGL
{
	/** Trivial representation of a rendering vertex containing world space, texture space, normal, and vertex color information
	*/
	struct Vertex
	{
		Vector3d pos_worldSpace;
		Vector2d pos_textureSpace;
		Vector3d normal;
	};
}
