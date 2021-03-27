#pragma once
#include <vector>
#include "InterRenderTriangle.h"

class ClippingEngine
{
public:
	static std::vector<InterRenderTriangle> Clip(const InterRenderTriangle& tri);

private:
	static void ClipEdge(uint8_t edge, InterRenderTriangle& tri, InterRenderTriangle& split_tri, bool& hasDroppedTri, bool& hasSplitTri);
	static uint8_t Outcode(const Vector4d& v);
	static double HomoDot(uint8_t edge, const Vector4d& v);
	static void ClipOneIn(uint8_t edge, InterRenderTriangle& tri, bool a_inside, bool b_inside, bool c_inside);
	static void ClipTwoIn(uint8_t edge, InterRenderTriangle& tri, InterRenderTriangle& split_tri, bool a_inside, bool b_inside, bool c_inside);
};

