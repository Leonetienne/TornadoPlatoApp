#include "RenderLightSource.h"
#include "Math.h"

void RenderLightSource::SetColor(const Color& color)
{
	this->color = color;
	return;
}

void RenderLightSource::SetIntensity(const double intensity)
{
	this->intensity = intensity;
	this->intensityTimes255 = intensity * 255.0;
	return;
}

void RenderLightSource::SetSoftness(const double softness)
{
	this->softness = Math::Clamp(softness, 0.0, 1.0);
	return;
}

void RenderLightSource::SetPosition(const Vector3d& position)
{
	this->position = position;
	return;
}

Color& RenderLightSource::GetColor()
{
	return color;
}

const Color& RenderLightSource::GetColor() const
{
	return color;
}

double RenderLightSource::GetIntensity() const
{
	return intensity;
}

double RenderLightSource::GetSoftness() const
{
	return softness;
}

Vector3d& RenderLightSource::GetPosition()
{
	return position;
}

const Vector3d& RenderLightSource::GetPosition() const
{
	return position;
}
