#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "Color.h"

struct Vertex
{
	Vector3d pos_worldSpace;
	Vector2d pos_textureSpace;
	Vector3d normal;
	Color vertexColor;
};
