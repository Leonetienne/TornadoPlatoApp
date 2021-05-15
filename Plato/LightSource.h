#pragma once
#include "Component.h"
#include "../Tornado/Color.h"
#include "../Tornado/RenderLightSource.h"

/** This component is the abstract implementation of all LightSources.
* You can't instanciate it on its own, and even if you did, bad things would happen. Don't do it.
*/
class LightSource : public Component
{
public:
	//! Will return a reference to the color
	virtual Color& GetColor();

	//! Will return a read-only reference to the color
	virtual const Color& GetColor() const;

	//! Will set the color
	virtual void SetColor(const Color& col);

	//! Will return the light intensity
	virtual const double GetIntensity() const;

	//! Will set the light intensity
	virtual void SetIntensity(double intensity);

	//! Will return the tornado render light source. This does NOT include transformation!
	virtual RenderLightSource* GetRawTornadoRenderLightSource() const = 0;

protected:
	mutable RenderLightSource* tornadoLightSource; // <-- Muteable so that the Renderer can access the position member

	friend class WorldObject;
	LightSource(WorldObject* worldObject, double intensity, const Color& color = Color::white);

private:
	void Render(Renderer* renderer) override;

};
