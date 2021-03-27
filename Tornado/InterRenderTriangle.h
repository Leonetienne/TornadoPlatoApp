#pragma once
#include "InterRenderVertex.h"
#include "Material.h"

struct InterRenderTriangle
{
	InterRenderVertex a;
	InterRenderVertex b;
	InterRenderVertex c;
	double ss_area = 0;	// Area of the screen space coordinate triangle

	Material* material = nullptr; // Material to render with
};
