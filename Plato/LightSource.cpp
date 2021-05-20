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
	return tornadoLightSource->GetColor();
}

const Color& LightSource::GetColor() const
{
	return tornadoLightSource->GetColor();
}

void LightSource::SetColor(const Color& col)
{
	tornadoLightSource->SetColor(col);
	return;
}

const double LightSource::GetIntensity() const
{
	return tornadoLightSource->GetIntensity();
}

void LightSource::SetIntensity(double intensity)
{
	tornadoLightSource->SetIntensity(intensity);
	return;
}

void LightSource::Render(Renderer* renderer)
{
	renderer->RegisterLightSource(this);
	return;
}
