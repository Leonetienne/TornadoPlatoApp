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
	uint8_t outcode_a = Outcode(tri.a.pos_cs);
	uint8_t outcode_b = Outcode(tri.b.pos_cs);
	uint8_t outcode_c = Outcode(tri.c.pos_cs);

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

	constexpr long long interpolationMask = IRV_LERP_POS_WS | IRV_LERP_POS_CS | IRV_LERP_POS_UV | IRV_LERP_NORMAL | IRV_LERP_VERTEX_COLOR;
	clippingResult[0].a.SetInterpolationMask(interpolationMask);
	clippingResult[0].b.SetInterpolationMask(interpolationMask);
	clippingResult[0].c.SetInterpolationMask(interpolationMask);

	// Iterate over edges
	// 0 -> left
	// 1 -> right
	// 2 -> top
	// 3 -> bottom
	// 4 -> near
	// 6 -> far
	for (uint8_t e = 0; e < 6; e++)
	{
		for (long t = (long)clippingResult.size() - 1; t >= 0; t--)
		{
			InterRenderTriangle splitTri;
			bool hasSplitTri;
			bool hasDroppedTriangle;
			ClipEdge(e, clippingResult[t], splitTri, hasDroppedTriangle, hasSplitTri);

			// Dump it
			if (hasDroppedTriangle)
				clippingResult.erase(clippingResult.begin() + t);

			// Oh my, we got an additional triangle for that face! Let's carry over the metadata!
			if (hasSplitTri)
			{
				splitTri.material = clippingResult[t].material;
				clippingResult.push_back(splitTri);
			}
		}
	}

	return clippingResult;
}

void ClippingEngine::ClipEdge(uint8_t edge, InterRenderTriangle& tri, InterRenderTriangle& split_tri, bool& hasDroppedTri, bool& hasSplitTri)
{
	hasSplitTri = false;
	hasDroppedTri = false;

	uint8_t outcode_a = Outcode(tri.a.pos_cs);
	uint8_t outcode_b = Outcode(tri.b.pos_cs);
	uint8_t outcode_c = Outcode(tri.c.pos_cs);

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
	const double da = HomoDot(edge, tri.a.pos_cs);
	const double db = HomoDot(edge, tri.b.pos_cs);
	const double dc = HomoDot(edge, tri.c.pos_cs);

	if (a_inside)
	{
		const double alphaB = (-da / (da - db)) + 1;
		const double alphaC = (-da / (da - dc)) + 1;

		tri.b.Interpolate(tri.a, alphaB);
		tri.c.Interpolate(tri.a, alphaC);
		//tri.csr_b = tri.csr_a.Lerp(tri.csr_b, alphaB);
		//tri.csr_c = tri.csr_a.Lerp(tri.csr_c, alphaC);
	}
	else if (b_inside)
	{
		const double alphaA = (-db / (db - da)) + 1;
		const double alphaC = (-db / (db - dc)) + 1;

		tri.a.Interpolate(tri.b, alphaA);
		tri.c.Interpolate(tri.b, alphaC);
		//tri.csr_a = tri.csr_b.Lerp(tri.csr_a, alphaA);
		//tri.csr_c = tri.csr_b.Lerp(tri.csr_c, alphaC);
	}
	else if (c_inside)
	{
		const double alphaB = (-dc / (dc - db)) + 1;
		const double alphaA = (-dc / (dc - da)) + 1;

		tri.a.Interpolate(tri.c, alphaA);
		tri.b.Interpolate(tri.c, alphaB);
		//tri.csr_a = tri.csr_c.Lerp(tri.csr_a, alphaA);
		//tri.csr_b = tri.csr_c.Lerp(tri.csr_b, alphaB);
	}

	return;
}

void ClippingEngine::ClipTwoIn(uint8_t edge, InterRenderTriangle& tri, InterRenderTriangle& split_tri, bool a_inside, bool b_inside, bool c_inside)
{
	const double da = HomoDot(edge, tri.a.pos_cs);
	const double db = HomoDot(edge, tri.b.pos_cs);
	const double dc = HomoDot(edge, tri.c.pos_cs);

	// AB inside
	if (!c_inside)
	{
		const double alphaCA = da / (da - dc);
		const double alphaCB = db / (db - dc);

		InterRenderVertex lerpedCA = tri.a;
		lerpedCA.Interpolate(tri.c, alphaCA);

		InterRenderVertex lerpedCB = tri.b;
		lerpedCB.Interpolate(tri.c, alphaCB);
		//const Vector4d lerpedCA = tri.csr_a.Lerp(tri.csr_c, alphaCA);
		//const Vector4d lerpedCB = tri.csr_b.Lerp(tri.csr_c, alphaCB);

		//tri.a = tri.a;
		//tri.b = tri.b;
		tri.c = lerpedCA;

		split_tri.a = tri.c;    // A becomes C'A
		split_tri.b = tri.b;    // B stays original B
		split_tri.c = lerpedCB; // C becomes C'B
	}
	// CB inside
	else if (!a_inside)
	{
		const double alphaAC = dc / (dc - da);
		const double alphaAB = db / (db - da);

		InterRenderVertex lerpedAC = tri.c;
		lerpedAC.Interpolate(tri.a, alphaAC);

		InterRenderVertex lerpedAB = tri.b;
		lerpedAB.Interpolate(tri.a, alphaAB);
		//const Vector4d lerpedAC = tri.csr_c.Lerp(tri.csr_a, alphaAC);
		//const Vector4d lerpedAB = tri.csr_b.Lerp(tri.csr_a, alphaAB);

		//tri.c = tri.c;
		//tri.b = tri.b;
		tri.a = lerpedAB;

		split_tri.a = lerpedAC;	// A becomes A'C 
		split_tri.b = tri.a;    // B becomes A'B
		split_tri.c = tri.c;    // C stays original C
	}
	// AC inside
	else if (!b_inside)
	{
		const double alphaBC = dc / (dc - db);
		const double alphaBA = da / (da - db);

		InterRenderVertex lerpedBC = tri.c;
		lerpedBC.Interpolate(tri.b, alphaBC);

		InterRenderVertex lerpedBA = tri.a;
		lerpedBA.Interpolate(tri.b, alphaBA);
		//const Vector4d lerpedBC = tri.csr_c.Lerp(tri.csr_b, alphaBC);
		//const Vector4d lerpedBA = tri.csr_a.Lerp(tri.csr_b, alphaBA);

		//tri.c = tri.c;
		//tri.a = tri.a;
		tri.b = lerpedBA;

		split_tri.a = tri.b;    // A becomes B'A
		split_tri.b = lerpedBC; // B becomes B'C
		split_tri.c = tri.c;	// C stays original C
	}

	return;
}

uint8_t ClippingEngine::Outcode(const Vector4d& v)
{
	uint8_t outcode = 0;

	for (uint8_t i = 0; i < 6; i++)
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
