#pragma once
#include "LightSource.h"

class PointLight : public LightSource
{
public:
	~PointLight();

	RenderLightSource* GetRawTornadoRenderLightSource() const override;

private:
	
	friend class WorldObject;
	PointLight(WorldObject* worldObject, double intensity, const Color& color = Color(255, 255, 255));
};
