#include "InterRenderTriangle.h"

bool InterRenderTriangle::DoesScreenspaceContainPoint(const Vector2d& p) const
{
	//double ef_ab = EdgeFunction2D(a.pos_ss, b.pos_ss, p);
	//double ef_ac = EdgeFunction2D(c.pos_ss, a.pos_ss, p);
	//double ef_bc = EdgeFunction2D(b.pos_ss, c.pos_ss, p);

	double ef_ab = EdgeFunction2D(b.pos_ss, a.pos_ss, p);
	double ef_ac = EdgeFunction2D(a.pos_ss, c.pos_ss, p);
	double ef_bc = EdgeFunction2D(c.pos_ss, b.pos_ss, p);

	return ((ef_ab >= 0) && (ef_ac >= 0) && (ef_bc >= 0));
}

double InterRenderTriangle::EdgeFunction2D(const Vector2d& a, const Vector2d& b, const Vector2d& p)
{
    return ((p.x - a.x) * (b.y - a.y)) - ((p.y - a.y) * (b.x - a.x));
}
