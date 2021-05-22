#pragma once
#include "Vector3.h"
#include <array>

class BoundingBox
{
public:
	BoundingBox();
	BoundingBox(const BoundingBox& other) = default;
	BoundingBox(BoundingBox&& other) noexcept = default;
	void operator=(const BoundingBox& other);
	void operator=(BoundingBox&& other) noexcept;

	//! Will return a specific vertex
	const Vector3d& GetVertex(std::size_t index) const;

	//! Will set the value of a specific vertex
	void SetVertex(std::size_t index, const Vector3d value);

	//! Tests, if this BoundingBox contains a point
	bool Contains(const Vector3d& point) const;

	/* Vertex identifiers */
	static constexpr std::size_t BACK	= 0;
	static constexpr std::size_t FRONT	= 4;
	static constexpr std::size_t LEFT	= 0;
	static constexpr std::size_t RIGHT	= 2;
	static constexpr std::size_t BOTTOM	= 0;
	static constexpr std::size_t TOP	= 1;

	Vector3d flb;
	Vector3d flt;
	Vector3d blb;
private:
	enum class FACE_NORMALS : std::size_t;

	//! Will calculate the vertex normals from vertices
	void GenerateNormalsFromVertices();

	//! Returns the dot product of a given point against a specific plane of the bounding box
	double FaceDot(FACE_NORMALS face, const Vector3d& point) const;

	std::array<Vector3d, 8> vertices;


	// Face normals
	enum class FACE_NORMALS : std::size_t
	{
		LEFT	= 0,
		RIGHT	= 1,
		FRONT	= 2,
		BACK	= 3,
		TOP		= 4,
		BOTTOM	= 5
	};
	std::array<Vector3d, 6> faceNormals;
};
