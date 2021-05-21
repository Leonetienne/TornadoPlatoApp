#include "BoundingBox.h"

const Vector3d& BoundingBox::GetVertex(std::size_t index) const
{
	return vertices[index];
}

void BoundingBox::SetVertex(std::size_t index, const Vector3d& value)
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

	case FACE_NORMALS::RIGHT:
		coreVertexIdx = FRONT|RIGHT|BOTTOM;
	
	case FACE_NORMALS::FRONT:
		coreVertexIdx = FRONT|LEFT|BOTTOM;

	case FACE_NORMALS::BACK:
		coreVertexIdx = BOTTOM|LEFT|BOTTOM;

	case FACE_NORMALS::TOP:
		coreVertexIdx = FRONT|LEFT|BOTTOM;

	case FACE_NORMALS::BOTTOM:
		coreVertexIdx = FRONT|LEFT|BOTTOM;
	}

	return faceNormals[(std::size_t)FACE_NORMALS::LEFT].DotProduct(point - vertices[coreVertexIdx]);
}

bool BoundingBox::Contains(const Vector3d& point) const
{
	for (std::size_t i = 0; i < 6; i++)
		if (FaceDot((FACE_NORMALS)i, point) < 0)
			return false;

	return true;
}
