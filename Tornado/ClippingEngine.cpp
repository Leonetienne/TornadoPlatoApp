#include "ClippingEngine.h"

/*
	Credits to https://chaosinmotion.com/2016/05/22/3d-clipping-in-homogeneous-coordinates/
	for nudging me in the right direction on that one!
	greatly appreciated!
*/


#define PlaneByOutcode(code, plane) (((uint8_t)code & (1 << plane)) ? 1 : 0)

std::vector<InterRenderTriangle> ClippingEngine::Clip(const InterRenderTriangle& tri)
{
	std::vector<InterRenderTriangle> clippingResult; // <-- Stores our triangles

	// Calculate outcodes for original triangle vertices
	uint8_t outcode_a = Outcode(tri.csr_a);
	uint8_t outcode_b = Outcode(tri.csr_b);
	uint8_t outcode_c = Outcode(tri.csr_c);

	// Fast accept. Triangle is completely visible
	if ((outcode_a | outcode_b | outcode_c) == 0)
	{
		clippingResult.push_back(tri);
		return clippingResult;
	}

	// Fast reject. Triangle is completely out of sight.
	// This being false does NOT mean the triangle is automatically visible!
	// This being true does just mean that all vertices are on wrong side of the same edge
	if (outcode_a & outcode_b & outcode_c)
		return clippingResult; // <- empty

	// If the triangle made it here, we actually have to clip it... *sigh*
	clippingResult.reserve(64); // Mathematically impossible to get more triangles out of one when clipping in three dimensions (homogenous coords are technically just distorted 3d coordinates)
	clippingResult.push_back(tri);

	// Iterate over edges
	// 0 -> left
	// 1 -> right
	// 2 -> top
	// 3 -> bottom
	// 4 -> near
	// 6 -> far
	for (std::size_t e = 0; e < 6; e++)
	{
		for (long t = clippingResult.size() - 1; t >= 0; t--)
		{
			InterRenderTriangle splitTri;
			bool hasSplitTri;
			bool hasDroppedTriangle;
			ClipEdge(e, clippingResult[t], splitTri, hasDroppedTriangle, hasSplitTri);

			if (hasDroppedTriangle)
				clippingResult.erase(clippingResult.begin() + t);
			if (hasSplitTri)
				clippingResult.push_back(splitTri);
		}
	}

	return clippingResult;
}

void ClippingEngine::ClipEdge(uint8_t edge, InterRenderTriangle& tri, InterRenderTriangle& split_tri, bool& hasDroppedTri, bool& hasSplitTri)
{
	hasSplitTri = false;
	hasDroppedTri = false;

	uint8_t outcode_a = Outcode(tri.csr_a);
	uint8_t outcode_b = Outcode(tri.csr_b);
	uint8_t outcode_c = Outcode(tri.csr_c);

	uint8_t numInside = 0;
	bool a_inside = false;
	bool b_inside = false;
	bool c_inside = false;
	if (!PlaneByOutcode(outcode_a, edge)) // Is edge for outcode_a not marked as outside?
	{
		numInside++;
		a_inside = true;
	}
	if (!PlaneByOutcode(outcode_b, edge))
	{
		numInside++;
		b_inside = true;
	}
	if (!PlaneByOutcode(outcode_c, edge))
	{
		numInside++;
		c_inside = true;
	}

	if (numInside == 0)
	{
		// None inside
		// Drop triangle
		hasDroppedTri = true;
		return;
	}
	else if (numInside == 3)
	{
		// All inside
		// Do nothing
		return;
	}
	else if (numInside == 2)
	{
		// Two of three inside
		// Split triangle
		hasSplitTri = true;
		ClipTwoIn(edge, tri, split_tri, a_inside, b_inside, c_inside);
	}
	else if (numInside == 1)
	{
		// One inside
		// Adjust outside vertices
		ClipOneIn(edge, tri, a_inside, b_inside, c_inside);
	}

	return;
}

void ClippingEngine::ClipOneIn(uint8_t edge, InterRenderTriangle& tri, bool a_inside, bool b_inside, bool c_inside)
{
	const double da = HomoDot(edge, tri.csr_a);
	const double db = HomoDot(edge, tri.csr_b);
	const double dc = HomoDot(edge, tri.csr_c);

	if (a_inside)
	{
		const double alphaB = da / (da - db);
		const double alphaC = da / (da - dc);

		tri.csr_b = tri.csr_a.Lerp(tri.csr_b, alphaB);
		tri.csr_c = tri.csr_a.Lerp(tri.csr_c, alphaC);
	}
	else if (b_inside)
	{
		const double alphaA = db / (db - da);
		const double alphaC = db / (db - dc);

		tri.csr_a = tri.csr_b.Lerp(tri.csr_a, alphaA);
		tri.csr_c = tri.csr_b.Lerp(tri.csr_c, alphaC);
	}
	else if (c_inside)
	{
		const double alphaB = dc / (dc - db);
		const double alphaA = dc / (dc - da);

		tri.csr_b = tri.csr_c.Lerp(tri.csr_b, alphaB);
		tri.csr_a = tri.csr_c.Lerp(tri.csr_a, alphaA);
	}

	return;
}

void ClippingEngine::ClipTwoIn(uint8_t edge, InterRenderTriangle& tri, InterRenderTriangle& split_tri, bool a_inside, bool b_inside, bool c_inside)
{
	const double da = HomoDot(edge, tri.csr_a);
	const double db = HomoDot(edge, tri.csr_b);
	const double dc = HomoDot(edge, tri.csr_c);

	// AB inside
	if (!c_inside)
	{
		const double alphaCA = da / (da - dc);
		const double alphaCB = db / (db - dc);

		const Vector4d lerpedCA = tri.csr_a.Lerp(tri.csr_c, alphaCA);
		const Vector4d lerpedCB = tri.csr_b.Lerp(tri.csr_c, alphaCB);

		tri.csr_a = tri.csr_a;
		tri.csr_b = tri.csr_b;
		tri.csr_c = lerpedCA;

		split_tri.csr_a = tri.csr_c; // A becomes C'A
		split_tri.csr_b = tri.csr_b; // B stays original B
		split_tri.csr_c = lerpedCB;  // C becomes C'B
	}
	// CB inside
	else if (!a_inside)
	{
		const double alphaAC = dc / (dc - da);
		const double alphaAB = db / (db - da);

		const Vector4d lerpedAC = tri.csr_c.Lerp(tri.csr_a, alphaAC);
		const Vector4d lerpedAB = tri.csr_b.Lerp(tri.csr_a, alphaAB);

		tri.csr_c = tri.csr_c;
		tri.csr_b = tri.csr_b;
		tri.csr_a = lerpedAB;

		split_tri.csr_a = lerpedAC;	 // A becomes A'C 
		split_tri.csr_b = tri.csr_a; // B becomes A'B
		split_tri.csr_c = tri.csr_c; // C stays original C
	}
	// AC inside
	else if (!b_inside)
	{
		const double alphaBC = dc / (dc - db);
		const double alphaBA = da / (da - db);

		const Vector4d lerpedBC = tri.csr_c.Lerp(tri.csr_b, alphaBC);
		const Vector4d lerpedBA = tri.csr_a.Lerp(tri.csr_b, alphaBA);

		tri.csr_c = tri.csr_c;
		tri.csr_a = tri.csr_a;
		tri.csr_b = lerpedBA;

		split_tri.csr_a = tri.csr_b; // A becomes B'A
		split_tri.csr_b = lerpedBC;  // B becomes B'C
		split_tri.csr_c = tri.csr_c; // C stays original C
	}

	return;
}

uint8_t ClippingEngine::Outcode(const Vector4d& v)
{
	uint8_t outcode = 0;

	for (std::size_t i = 0; i < 6; i++)
	{
		if (HomoDot(i, v) <= 0)
		{
			outcode |= (1 << i);
		}
	}

	return outcode;
}

double ClippingEngine::HomoDot(uint8_t edge, const Vector4d& v)
{
	switch (edge) {
	case 0: // left
		return -v.x + v.w;
	case 1: // right
		return v.x + v.w;
	case 2: // top
		return -v.y + v.w;
	case 3: // bottom
		return v.y + v.w;
	case 4: // near
		return v.z;
	case 5: // far
		return -v.z + v.w;
	}

	return 0.0; // Should not come to this point
}
