#pragma once
#include "Vertex.h"
#include "Material.h"

struct RenderTriangle3D
{
	Vertex a;
	Vertex b;
	Vertex c;

	Material* material; // Material to render with
};
