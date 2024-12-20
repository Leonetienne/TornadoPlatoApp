#pragma once
#include "Color.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include <vector>

#define IRV_LERP_POS_WS       (1<<0)
#define IRV_LERP_POS_WSMX     (1<<1)
#define IRV_LERP_POS_CS       (1<<2)
#define IRV_LERP_POS_NDC      (1<<3)
#define IRV_LERP_POS_SS       (1<<4)
#define IRV_LERP_POS_UV       (1<<5)
#define IRV_LERP_NORMAL       (1<<6)

namespace TorGL
{
	/** A vertex representation for within the rendering pipeline.
	* This struct contains a lot of cached values. We have much more ram than processing time!
	*/
	struct InterRenderVertex
	{
		Vector3d pos_ws;  //! Position in world space
		Vector3d pos_wsmx;//! Position in world space after applying the world matrix
		Vector4d pos_cs;  //! Position in clipping space (before perspective divide)
		Vector3d pos_ndc; //! Position in normalized device coordinates (after perspective divide)
		Vector3d pos_ss;  //! Position in screen space (pixel space)
		Vector2d pos_uv;  //! Positionin texture space

		Vector3d normal;  //! Normal

		mutable double berp_iw = -1; //! 1.0 / pos_cs.z caching value. Used by the barycentric interpolation engine to only calculate it once per triangle instead of every pixel

		//! Determines which attributes to interpolate when calling Interpolate.  
		//! Use the macros defined with the prefix IRV_, concatenated via | bitwise or
		void SetInterpolationMask(long long mask);

		//! Will return the interpolation mask
		long long GetInterpolationMask() const;

		//! This is ONLY for interpolating between two vertices!
		//! NOT to interpolate within a triangle, between three vertices!!
		void Interpolate(const InterRenderVertex& b, double t);

	private:
		long long interpolationMask = 0;
	};
}
