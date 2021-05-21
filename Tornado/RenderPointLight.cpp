#include "RenderPointLight.h"
#include "Math.h"

Color RenderPointLight::GetColorIntensityFactors(const InterRenderTriangle* ird, const Vector3d& point, const Vector3d& normal) const
{
	const Vector3d deltaPos = position - point;
	const double sqrDistance = deltaPos.SqrMagnitude();

	if ((useBoundingBox) && (!Inside(point)))
		return Color::black;

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

double RenderPointLight::Dot(uint8_t side, const Vector3d& p) const
{
	switch (side)
	{
		// left
	case 0:
		return (box.blb - box.flb).CrossProduct(box.flt - box.flb).DotProduct(p - box.flb);
	
		// right
	case 1:
		return (box.frt - box.frb).CrossProduct(box.brb - box.frb).DotProduct(p - box.frb);
	
		// front
	case 2:
		return (box.flt - box.flb).CrossProduct(box.frb - box.flb).DotProduct(p - box.flb);
	
		// back
	case 3:
		return (box.brb - box.blb).CrossProduct(box.blt - box.blb).DotProduct(p - box.blb);
	
		// top
	case 4:
		return (box.blt - box.flt).CrossProduct(box.frt - box.flt).DotProduct(p - box.flt);
	
		// bottom
	case 5:
		return (box.frb - box.flb).CrossProduct(box.blb - box.flb).DotProduct(p - box.flb);
	}

	return 0.0;
}

bool RenderPointLight::Inside(const Vector3d& p) const
{
	for (std::size_t i = 0; i < 6; i++)
		if (Dot(i, p) < 0)
			return false;
		
	return true;
}
