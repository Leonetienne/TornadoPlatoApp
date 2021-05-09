#pragma once
#include <vector>
#include "../Tornado/Vector2.h"
#include "../Tornado/Vector3.h"

//! This struct holds indices for all important vertex types.
struct MeshVertexIndices
{
	//! Index of the 3D world vertex
	std::size_t v;

	// Index of the uv (texture space) vertex
	std::size_t uv;

	// Index of the normal value
	std::size_t vn;
};

/** 3D mesh representation.
*/
struct Mesh
{
	std::vector<Vector3d> v_vertices;
	std::vector<Vector2d> uv_vertices;
	std::vector<Vector3d> normals;
	std::vector<MeshVertexIndices> tris;
};
