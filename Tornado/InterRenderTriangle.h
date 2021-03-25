#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Material.h"

struct InterRenderTriangle
{
	Vector3d ws_a; // World space position vertex a
	Vector3d ws_b; // World space position vertex b
	Vector3d ws_c; // World space position vertex c

	Vector4d csr_a;	// Clipping space raw position vertex a
	Vector4d csr_b;	// Clipping space raw position vertex b
	Vector4d csr_c;	// Clipping space raw position vertex c

	Vector3d ndc_a;	// Normalized device coordinates vertex a
	Vector3d ndc_b;	// Normalized device coordinates vertex b
	Vector3d ndc_c;	// Normalized device coordinates vertex c

	Vector2d ss_a;	// Screen space coordinates vertex a
	Vector2d ss_b;	// Screen space coordinates vertex b
	Vector2d ss_c;	// Screen space coordinates vertex c
	double ss_area;	// Area of the screen space coordinate triangle

	Vector2i uv_a;	// Texture space position for vertex a
	Vector2i uv_b;	// Texture space position for vertex b
	Vector2i uv_c;	// Texture space position for vertex c

	Material* material; // Material to render with
};
