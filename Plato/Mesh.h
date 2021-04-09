#pragma once
#include <vector>
#include "../Tornado/Vector2.h"
#include "../Tornado/Vector3.h"

struct MeshVertexIndices
{
	std::size_t v;
	std::size_t uv;
	std::size_t vn;
};

struct Mesh
{
	std::vector<Vector3d> v_vertices;
	std::vector<Vector2d> uv_vertices;
	std::vector<Vector3d> normals;
	std::vector<MeshVertexIndices> tris;
};
