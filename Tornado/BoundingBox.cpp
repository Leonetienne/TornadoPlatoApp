#include "BoundingBox.h"
#include <iostream>

BoundingBox::BoundingBox()
{
	return;
}

void BoundingBox::operator=(const BoundingBox& other)
{
	vertices = other.vertices;
	faceNormals = other.faceNormals;
	flb = other.flb;
	flt = other.flt;
	blb = other.blb;
	return;
}

void BoundingBox::operator=(BoundingBox&& other) noexcept
{
	vertices = std::move(other.vertices);
	faceNormals = std::move(other.faceNormals);
	flb = Vector3d(other.flb);
	flt = Vector3d(other.flt);
	blb = Vector3d(other.blb);
	return;
}

const Vector3d& BoundingBox::GetVertex(std::size_t index) const
{
	return vertices[index];
}

void BoundingBox::SetVertex(std::size_t index, const Vector3d value)
{
	vertices[index] = value;
	GenerateNormalsFromVertices();
	return;
}

void BoundingBox::GenerateNormalsFromVertices()
{
	faceNormals[(std::size_t)FACE_NORMALS::LEFT] =
		(vertices[BACK|LEFT|BOTTOM] - vertices[FRONT|LEFT|BOTTOM])
		.CrossProduct(vertices[FRONT|LEFT|TOP] - vertices[FRONT|LEFT|BOTTOM]);
	
	faceNormals[(std::size_t)FACE_NORMALS::RIGHT] =
		(vertices[FRONT|RIGHT|TOP] - vertices[FRONT|RIGHT|BOTTOM])
		.CrossProduct(vertices[BACK|RIGHT|BOTTOM] - vertices[FRONT|RIGHT|BOTTOM]);

	faceNormals[(std::size_t)FACE_NORMALS::FRONT] =
		(vertices[FRONT|LEFT|TOP] - vertices[FRONT|LEFT|BOTTOM])
		.CrossProduct(vertices[FRONT|RIGHT|BOTTOM] - vertices[FRONT|LEFT|BOTTOM]);

	faceNormals[(std::size_t)FACE_NORMALS::BACK] =
		(vertices[BACK|RIGHT|BOTTOM] - vertices[BACK|LEFT|BOTTOM])
		.CrossProduct(vertices[BACK|LEFT|TOP] - vertices[BACK|LEFT|BOTTOM]);

	faceNormals[(std::size_t)FACE_NORMALS::TOP] =
		(vertices[BACK|LEFT|TOP] - vertices[FRONT|LEFT|TOP])
		.CrossProduct(vertices[FRONT|RIGHT|TOP] - vertices[FRONT|LEFT|TOP]);

	faceNormals[(std::size_t)FACE_NORMALS::BOTTOM] =
		(vertices[FRONT|RIGHT|BOTTOM] - vertices[FRONT|LEFT|BOTTOM])
		.CrossProduct(vertices[BACK|LEFT|BOTTOM] - vertices[FRONT|LEFT|BOTTOM]);

	return;
}

double BoundingBox::FaceDot(FACE_NORMALS face, const Vector3d& point) const
{
	// This vertex is the one being used twice to calculate the normals
	std::size_t coreVertexIdx;
	switch (face)
	{
	case FACE_NORMALS::LEFT:
		coreVertexIdx = FRONT|LEFT|BOTTOM;
		break;

	case FACE_NORMALS::RIGHT:
		coreVertexIdx = FRONT|RIGHT|BOTTOM;
		break;
	
	case FACE_NORMALS::FRONT:
		coreVertexIdx = FRONT|LEFT|BOTTOM;
		break;
	
	case FACE_NORMALS::BACK:
		coreVertexIdx = BACK|LEFT|BOTTOM;
		break;
	
	case FACE_NORMALS::TOP:
		coreVertexIdx = FRONT|LEFT|TOP;
		break;
	
	case FACE_NORMALS::BOTTOM:
		coreVertexIdx = FRONT|LEFT|BOTTOM;
		break;
	}

	if ((std::size_t)face < 6)
		return faceNormals[(std::size_t)face].DotProduct(point - vertices[coreVertexIdx]);
	return 1;
}

bool BoundingBox::Contains(const Vector3d& point) const
{
	for (std::size_t i = 0; i < 6; i++)
		if (FaceDot((FACE_NORMALS)i, point) < 0)
			return false;

	//return faceNormals[(std::size_t)FACE_NORMALS::LEFT]
	//	.DotProduct(point - vertices[FRONT|LEFT|BOTTOM]) >= 0;

	//return (vertices[BACK | LEFT | BOTTOM] - vertices[FRONT | LEFT | BOTTOM])
	//	   .CrossProduct(vertices[FRONT | LEFT | TOP] - vertices[FRONT | LEFT | BOTTOM])
	//	   .DotProduct(point - vertices[FRONT | LEFT | BOTTOM]) >= 0;

	//return (blb - flb).CrossProduct(flt - flb).DotProduct(point - flb) >= 0;
	//
	//return ((blb - flb)
	//	.CrossProduct(flt - flb)
	//	.DotProduct(point - flb)) >= 0;

	return true;
}
