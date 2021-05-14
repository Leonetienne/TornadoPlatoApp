#include "PointLight.h"
#include "../Tornado/RenderPointLight.h"

PointLight::PointLight(WorldObject* worldObject, double intensity, const Color& color)
	:
	LightSource(worldObject, intensity, color)
{
	tornadoLightSource = new RenderPointLight();
	tornadoLightSource->color = color;
	tornadoLightSource->intensity = intensity;
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
