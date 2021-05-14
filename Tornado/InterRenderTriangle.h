#pragma once
#include "InterRenderVertex.h"
#include "Material.h"

/** A triangle (face) representation for within the rendering pipeline.
* This is basically a later form of the RenderTriangle3D struct, but with a LOT more values.
* These values are cached intermediate calculation steps that may be needed further down the rendering pipeline.
* We have much more ram than processing time, so save that time!
*/
struct InterRenderTriangle
{
	InterRenderVertex a;
	InterRenderVertex b;
	InterRenderVertex c;

	mutable double ss_area = -1;	//! Cached value. Area
	mutable double ss_iarea = -1;	//! Cached value. Inverted area
	mutable Vector3d surfaceNormalNdc = Vector3d::zero; //! Cached value. Surface normal (relative to normalized device coordinate space). Calculated in BackfaceCullingEngine::Cull()
	mutable Vector3d meanVertexNormal = Vector3d::zero; //! Cached value. Mean vertex normal according to mesh data. Calculated in DrawingEngine::CalculateRenderingRelatedCaches_IRD()

	const Material* material = nullptr; //! Material to render with

	//! Is a screen space point contained within this triangle?  
	//! Undefined behaviour, if the triangle has not yet been projected to screen space.
	bool DoesScreenspaceContainPoint(const Vector2d& p) const;

private:
	static double EdgeFunction2D(const Vector2d& a, const Vector2d& b, const Vector2d& p);
};
