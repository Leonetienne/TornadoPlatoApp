#include "PointLight.h"
#include "../Tornado/RenderPointLight.h"

using namespace Plato;
using namespace Plato::Components;
using namespace TorGL;

PointLight::PointLight(WorldObject* worldObject, double intensity, const Color& color)
	:
	LightSource(worldObject, intensity, color)
{
	tornadoLightSource = new RenderPointLight();
	tornadoLightSource->SetColor(color);
	tornadoLightSource->SetIntensity(intensity);
}


PointLight::~PointLight()
{
	delete tornadoLightSource;
	tornadoLightSource = nullptr;
	
	return;
}

RenderLightSource* PointLight::GetRawTornadoRenderLightSource() const
{
	return tornadoLightSource;
}
