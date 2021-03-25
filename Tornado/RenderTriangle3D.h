#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "Material.h"

struct RenderTriangle3D
{
	Vector3d ws_a; // World space position vertex a
	Vector3d ws_b; // World space position vertex b
	Vector3d ws_c; // World space position vertex c

	Vector2i uv_a; // Texture space position for vertex a
	Vector2i uv_b; // Texture space position for vertex b
	Vector2i uv_c; // Texture space position for vertex c

	Material* material; // Material to render with
};
