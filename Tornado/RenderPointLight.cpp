#include "RenderPointLight.h"

Color RenderPointLight::GetColorIntensityFactors(const InterRenderTriangle* ird, const Vector3d& point) const
{
	const Vector3d deltaPos = position - point;
	const double sqrDistance = (deltaPos).SqrMagnitude();

	// Too far away.
	if (intensity*13 < sqrDistance)
		return Color::black;

	// Angle of the face towards the point light
	const double dot = (deltaPos.Normalize()).DotProduct(ird->surfaceNormalWs);

	// Not facing the point light? No light for you! >:(
	if (dot < 0)
		return Color::black;

	// Point is in range
	const double invSqrCoefficient = intensity / sqrDistance;
	const double fac = (invSqrCoefficient / 10.0) * dot;

	return Color(
		color.r * fac,
		color.g * fac,
		color.b * fac,
		255
	);
}
