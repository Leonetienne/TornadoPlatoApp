#include "RenderPointLight.h"
#include "Math.h"

Color RenderPointLight::GetColorIntensityFactors(const InterRenderTriangle* ird, const Vector3d& point, const Vector3d& normal) const
{
	const Vector3d deltaPos = position - point;
	const double sqrDistance = deltaPos.SqrMagnitude();

	// Too far away.
	if (intensityTimes255 <= sqrDistance)
		return Color::black;

	// Too close
	if (sqrDistance == 0)
		return color;

	// Angle of the face towards the point light
	const double dot = (deltaPos.Normalize()).DotProduct(normal);

	// Brightness factor based purely on intensity and distance
	const double invSqrCoefficient = intensity / sqrDistance;

	// Calculate brightness factor including face angle and soft/hardness
	double fac;
	// No softness? We need no softlight.
	if (softness == 0.0)
		fac = GetHardlightFac(dot, invSqrCoefficient);

	// No hardness? We need no hardlight.
	else if (softness == 1.0)
		fac = GetSoftlightFac(invSqrCoefficient);

	// Okay.. it's an inbetween
	else
		fac = Math::Lerp(
			GetHardlightFac(dot, invSqrCoefficient),
			GetSoftlightFac(invSqrCoefficient),
			softness
		);

	return Color(
		color.r * fac,
		color.g * fac,
		color.b * fac,
		255
	);
}

double RenderPointLight::GetHardlightFac(const double dot, const double invSqrCoefficient) const
{
	if (dot < 0)
		return 0;

	return invSqrCoefficient * dot;
}

double RenderPointLight::GetSoftlightFac(const double invSqrCoefficient) const
{
	return invSqrCoefficient;
}
