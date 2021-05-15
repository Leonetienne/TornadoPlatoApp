#pragma once
#include "LightSource.h"

/** This component acts as a simple light source, shining in all directions.
* Note that this is a simple light source, and does not cast any shadows.
*/
class PointLight : public LightSource
{
public:
	~PointLight();

	//! Will return the tornado render light source. This will be an instance of RenderPointLight. This does NOT include transformation!
	RenderLightSource* GetRawTornadoRenderLightSource() const override;

private:
	
	friend class WorldObject;
	PointLight(WorldObject* worldObject, double intensity, const Color& color = Color::white);
};