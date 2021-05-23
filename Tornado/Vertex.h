#pragma once
#include "../Eule/Vector2.h"
#include "../Eule/Vector3.h"
#include "Color.h"

/** Trivial representation of a rendering vertex containing world space, texture space, normal, and vertex color information
*/
struct Vertex
{
	Vector3d pos_worldSpace;
	Vector2d pos_textureSpace;
	Vector3d normal;
};
