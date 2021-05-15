#include "LightSource.h"
#include "Renderer.h"

LightSource::LightSource(WorldObject* worldObject, double intensity, const Color& color) :
	Component(worldObject),
	tornadoLightSource { nullptr }
{

	return;
}

Color& LightSource::GetColor()
{
	return tornadoLightSource->color;
}

const Color& LightSource::GetColor() const
{
	return tornadoLightSource->color;
}

void LightSource::SetColor(const Color& col)
{
	tornadoLightSource->color = col;
	return;
}

const double LightSource::GetIntensity() const
{
	return tornadoLightSource->intensity;
}

void LightSource::SetIntensity(double intensity)
{
	tornadoLightSource->intensity = intensity;
	return;
}

void LightSource::Render(Renderer* renderer)
{
	renderer->RegisterLightSource(this);
}
