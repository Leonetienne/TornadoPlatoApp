#pragma once
#include "InterRenderVertex.h"
#include "Material.h"

struct InterRenderTriangle
{
	InterRenderVertex a;
	InterRenderVertex b;
	InterRenderVertex c;
	mutable double ss_iarea = -1;	// Cached value. Inverted area

	Material* material = nullptr; // Material to render with

	bool DoesScreenspaceContainPoint(const Vector2d& p) const;

private:
	static double EdgeFunction2D(const Vector2d& a, const Vector2d& b, const Vector2d& p);
};
