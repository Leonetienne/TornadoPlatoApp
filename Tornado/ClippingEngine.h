#pragma once
#include <vector>
#include "InterRenderTriangle.h"

/** Clips InterRenderTriangles to the clipping space.
* 
* This class is used to split a single triangle into 0-n new triangles that represent the original triangle
* clipped inside the clipping space. This class also interpolates vertex-metadata such as normals and
* texture coordinates. Other data, such as material data, will be preserved.
*   
* Thanks to https://chaosinmotion.com/2016/05/22/3d-clipping-in-homogeneous-coordinates/ for being a big
* help on this one!
*/
class ClippingEngine
{
public:
	//! Will clip an InterRenderTriangle into clipping space. May return \f$[0-n]\f$ triangles as a result
	static std::vector<InterRenderTriangle> Clip(const InterRenderTriangle& tri);

private:
	//! Will clip an InterRenderTriangle against a single edge, or more like... plane
	static void ClipEdge(uint8_t edge, InterRenderTriangle& tri, InterRenderTriangle& split_tri, bool& hasDroppedTri, bool& hasSplitTri);
	
	//! Will return a bitmap of describing the planes the homogeneous vector `v` lays outside
	static uint8_t Outcode(const Vector4d& v);

	//! Homogeneous dot product against an edge / plane.
	static double HomoDot(uint8_t edge, const Vector4d& v);
	
	//! Specialized method to clip an InterRenderTriangle against an edge/plane, ONLY with one vertex inside.
	static void ClipOneIn(uint8_t edge, InterRenderTriangle& tri, bool a_inside, bool b_inside, bool c_inside);
	
	//! Specialized method to clip an InterRenderTriangle against an edge/plane, ONLY with two vertices inside.
	static void ClipTwoIn(uint8_t edge, InterRenderTriangle& tri, InterRenderTriangle& split_tri, bool a_inside, bool b_inside, bool c_inside);
};

