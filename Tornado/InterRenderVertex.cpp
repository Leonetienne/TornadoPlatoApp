#include "InterRenderVertex.h"

void InterRenderVertex::SetInterpolationMask(long long mask)
{
	interpolationMask = mask;
	return;
}

long long InterRenderVertex::GetInterpolationMask() const
{
	return interpolationMask;
}

void InterRenderVertex::Interpolate(const InterRenderVertex& b, double t)
{
	if (interpolationMask & IRV_LERP_POS_WS)
	{
		pos_ws.LerpSelf(b.pos_ws, t);
	}
	if (interpolationMask & IRV_LERP_POS_WSMX)
	{
		pos_wsmx.LerpSelf(b.pos_wsmx, t);
	}
	if (interpolationMask & IRV_LERP_POS_CS)
	{
		pos_cs.LerpSelf(b.pos_cs, t);
	}
	if (interpolationMask & IRV_LERP_POS_NDC)
	{
		pos_ndc.LerpSelf(b.pos_ndc, t);
	}
	if (interpolationMask & IRV_LERP_POS_SS)
	{
		pos_ss.LerpSelf(b.pos_ss, t);
	}
	if (interpolationMask & IRV_LERP_POS_UV)
	{
		pos_uv.LerpSelf(b.pos_uv, t);
	}
	if (interpolationMask & IRV_LERP_NORMAL)
	{
		normal.LerpSelf(b.normal, t);
	}
	if (interpolationMask & IRV_LERP_VERTEX_COLOR)
	{
		vertex_col.LerpSelf(b.vertex_col, t);
	}
	return;
}
