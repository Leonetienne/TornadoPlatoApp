#pragma once
#include <vector>
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Color.h"

#define IRV_LERP_POS_WS 0x01
#define IRV_LERP_POS_CS 0x02
#define IRV_LERP_POS_NDC 0x04
#define IRV_LERP_POS_SS 0x08
#define IRV_LERP_POS_UV 0x10
#define IRV_LERP_NORMAL 0x20
#define IRV_LERP_VERTEX_COLOR 0x40

struct InterRenderVertex
{
	Vector3d pos_ws;  // Position in world space
	Vector4d pos_cs;  // Position in clipping space (before perspective divide)
	Vector3d pos_ndc; // Position in normalized device coordinates (after perspective divide)
	Vector2d pos_ss;  // Position in screen space (pixel space)
	Vector2d pos_uv;  // Positionin texture space
	
	Vector3d normal;  // Normal
	Color vertex_col; // Vertex color

	mutable double berp_iw = -1; // 1.0 / pos_cs.z caching value. Used by the barycentric interpolation engine to only calculate it once per triangle instead of every pixel

	// Determines which attributes to interpolate when calling Interpolate
	void SetInterpolationMask(long long mask);
	long long GetInterpolationMask() const;

	// This is ONLY for interpolating between two vertices!
	// NOT to interpolate within a triangle, between three vertices!!
	void Interpolate(const InterRenderVertex& b, double t);

private:
	long long interpolationMask = 0;
};
