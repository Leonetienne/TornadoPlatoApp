#include "RenderPointLight.h"

Color RenderPointLight::GetColorIntensityFactors(const InterRenderTriangle* ird, const Vector3d& point) const
{
	const double sqrDistance = (position - point).SqrMagnitude();

	// Too far away.
	if (intensity*13 < sqrDistance)
		return Color(0, 0, 0, 0);

	// Point is in range
	const double invSqrCoefficient = intensity / sqrDistance;
	const double fac = std::min(invSqrCoefficient / 10, 1.0);

	return Color(
		color.r * fac,
		color.g * fac,
		color.b * fac,
		255
	);
}
