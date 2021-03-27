#pragma once
#include "InterRenderVertex.h"
#include "Material.h"

struct InterRenderTriangle
{
	InterRenderVertex a;
	InterRenderVertex b;
	InterRenderVertex c;
	double ss_area;	// Area of the screen space coordinate triangle

	Material* material; // Material to render with
};
