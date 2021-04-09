#pragma once
#include "Vertex.h"
#include "Material.h"

struct RenderTriangle3D
{
	Vertex a;
	Vertex b;
	Vertex c;

	const Material* material; // Material to render with
};
